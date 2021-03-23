#include "huffman_tree.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>

#include "input_helper.h"

huffman_tree::huffman_tree(const std::unordered_map <char, int>& occurrence_count_data) : root(nullptr)
{
	std::vector<huffman_node> nodes;

	/* create lambda helper function to help us merge 2 smallest nodes */
	auto merge_two_smallest_nodes = [&]()
	{
		assert(nodes.size() >= 2); // we need at least 2 elements to merge

		huffman_node first = std::move(nodes.back()); // move constructor
		nodes.pop_back();

		huffman_node second = std::move(nodes.back()); // move constructor
		nodes.pop_back();

		nodes.emplace_back(first.data + second.data, std::move(first), std::move(second)); // make sure no data is copied
	};

	for (const auto& it : occurrence_count_data)
	{
		/* occurrence_data: ('e', 2) ('a', 3) ('d', 4) ('b', 5) ('c', 6) */

		/* create initial leaf nodes */
		nodes.emplace_back(it.second, it.first);
	}

	/* merge nodes until we have 1 node left */
	while (nodes.size() >= 2)
	{
		/* sort the nodes regarding to their numbers descending order */
		std::sort(nodes.begin(), nodes.end(), [](const huffman_node& a, const huffman_node& b) -> bool
			{
				return a.data > b.data;
			});

		/* just merge last 2 nodes together, since it is sorted, it will be smallest 2 numbers */
		merge_two_smallest_nodes();
	}

	/* we should only have the root node now (all other nodes are inside root's hierarchy) */
	assert(nodes.size() == 1);

	root = new huffman_node(std::move(nodes.back())); // move constructor
	nodes.pop_back();

	/* prepare for encoding*/
	build_encoding_table();
}

huffman_tree::huffman_tree(std::ifstream& serialized_tree_file) : root(nullptr)
{
	deserialize_internal(serialized_tree_file, root);

	/* prepare for decoding */
	build_encoding_table();
	build_reverse_encoding_table();
}


std::string huffman_tree::encode_message(std::string original)
{
	std::stringstream result;

	for (char const& c : original)
	{
		/* for each character */
		result << encoding_table[c];
	}

	return result.str();
}

std::string huffman_tree::decode_message(std::string encoded)
{
	std::stringstream result;
	if (root->is_leaf())
	{
		/* handle the edge case when we have only 1 character */
		assert(reverse_encoding_table.size() == 1);
		return std::string(encoded.size(), reverse_encoding_table[std::string(1, encoded[0])]);
	}
	decode_message_internal(root, result, "", std::move(encoded));

	return result.str();
}

void huffman_tree::build_encoding_table()
{
	if (root->is_leaf())
	{
		/* handle the edge case when input file contains only 1 bit or 1 character */
		//std::cout << "[WARNING] Input file contains only 1 bit or 1 character!" << std::endl;
		encoding_table[root->character] = "0";
		return;
	}
	build_encoding_table_internal(root, {});
}

void huffman_tree::build_reverse_encoding_table()
{
	assert(!encoding_table.empty());
	reverse_encoding_table.clear();

	std::for_each(encoding_table.begin(), encoding_table.end(),
		[&](const std::pair<char, std::string>& p)
		{
			//std::cout << "Reverse encoding of " << p.second << " is " << p.first << std::endl;
			reverse_encoding_table.insert(std::make_pair(p.second, p.first));
		});
}

void huffman_tree::serialize(std::ofstream& file)
{
	serialize_internal(file, root);
}

void huffman_tree::print()
{
	print_internal("", root, true);
}

std::string huffman_tree::get_encoding_for_character(char character)
{
	assert(!encoding_table.empty());
	return encoding_table[character];
}

huffman_tree::huffman_node::huffman_node() : data(), character(), left_child(nullptr), right_child(nullptr)
{

}

/* this constructor does not need character because it does not construct nodes that can be leafs. */
huffman_tree::huffman_node::huffman_node(int number, char character_value) : character(character_value), data(number),
left_child(nullptr), right_child(nullptr)
{

}

huffman_tree::huffman_node::huffman_node(int number, huffman_node left, huffman_node right) : character(), data(number),
left_child(new huffman_node(std::move(left))), right_child(new huffman_node(std::move(right)))
{

}

void huffman_tree::build_encoding_table_internal(huffman_node* node, std::vector<bool> bit_storage)
{
	/* end */
	if (!node)
		return;

	/* check if we are a leaf */
	if (node->is_leaf())
	{
		std::stringstream ss;
		for (const auto& bit : bit_storage)
			ss << bit;

		encoding_table[node->character] = ss.str();
		//std::cout << node->data << "- " << node->character << ": " << ss.str() << std::endl;
		return;
	}

	/* recursively check left child */
	if (node->left_child)
	{
		bit_storage.push_back(0);
		build_encoding_table_internal(node->left_child, bit_storage);
		bit_storage.pop_back(); // pop the added 0 because we might go traverse right after
	}

	/* recursively check right child */
	if (node->right_child)
	{
		bit_storage.push_back(1);
		build_encoding_table_internal(node->right_child, bit_storage);
	}
}

void huffman_tree::serialize_internal(std::ofstream& file, huffman_node* node)
{
	if (!node)
	{
		file << "-1 "; // end
		return;
	}
	
	if (node->is_leaf())
	{
		/* handle special characters */
		bool reserved = false;
		for (int i = 0; i < reserved_character_count; i++)
		{
			if (node->character == reserved_chars[i])
			{
				reserved = true;
				// std::cout << "found reserved character: " << node->character << ", replacing with:"
				// << escape_strings[i] << std::endl;
				file << node->data << "|" << escape_strings[i] << " ";
				break;
			}
		}

		if (!reserved)
			file << node->data << "|" << node->character << " ";
	}
	else
		file << node->data << " ";

	serialize_internal(file, node->left_child);
	serialize_internal(file, node->right_child);
}

void huffman_tree::deserialize_internal(std::ifstream& file, huffman_node*& node)
{
	char character = ' ';
	int value = 0;

	std::string part;
	file >> part;

	if (part.find('|') != std::string::npos)
	{
		/* leaf node with character value */
		auto splitted = input_helper::split(part, '|');
		value = std::stoi(splitted[0]);
		auto char_part = splitted[1];
		if (char_part.size() > 1)
		{
			/* escape character detected, restore original value from reserved_chars */
			for (int i = 0; i < reserved_character_count; i++)
			{
				if (char_part == escape_strings[i])
				{				
					character = reserved_chars[i];
					//std::cout << "found escape string : " << char_part << ", replacing with original "
						//<< character << std::endl;
					break;
				}
			}
		}
		else
			character = splitted[1][0];
	}
	else
	{
		/* just integer */
		value = std::stoi(part);
	}

	if (value == -1) // end
		return;

	node = new huffman_node(value, character);
	deserialize_internal(file, node->left_child);
	deserialize_internal(file, node->right_child);
}

void huffman_tree::decode_message_internal(huffman_node* node, std::stringstream& output_stream,
	std::string done_encoded, std::string remaining_encoded)
{
	if (node->is_leaf())
	{
		output_stream << reverse_encoding_table[done_encoded];

		/* lookup finished for this sequence of bits, reset node to root, also clear done_encoded buffer */
		node = root;
		done_encoded.clear();
	}

	if (remaining_encoded.empty())
	{
		/* finished decoding the bits */
		return;
	}

	auto next = remaining_encoded[0];
	remaining_encoded.erase(remaining_encoded.begin()); // remove first bit
	if (next == '0')
	{
		/* go left */
		decode_message_internal(node->left_child, output_stream, done_encoded + '0', remaining_encoded);
	}
	else
	{
		/* go right */
		decode_message_internal(node->right_child, output_stream, done_encoded + '1', remaining_encoded);
	}
}

void huffman_tree::print_internal(const std::string& prefix, huffman_node* node, bool is_right)
{
	if (!node)
		return;

	std::cout << prefix;
	std::cout << "+- ";
	std::cout << node->data;
	if (node->is_leaf())
		std::cout << "[" << node->character << "]" << std::endl;
	std::cout << std::endl;


	print_internal(prefix + (is_right ? "    " : "|   "), node->left_child, false);
	print_internal(prefix + (is_right ? "    " : "|   "), node->right_child, true);
}
