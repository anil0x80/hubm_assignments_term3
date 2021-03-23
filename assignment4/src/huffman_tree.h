#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


class huffman_tree
{
public:
	/* construct huffman tree from occurrence data of characters */
	huffman_tree(const std::unordered_map <char, int>& occurrence_count_data);

	/* construct huffman tree from file stream */
	huffman_tree(std::ifstream& serialized_tree_file);
	
	~huffman_tree()
	{
		/* free memory, keep in mind if tree is HUGE, this can lead to
		 * stack overflow, since all of the destructors of root will be called recursively */
		delete root;
	}

	/* returns the encoded original message as encoded bit stream as string, example: "110100001010100" */
	std::string encode_message(std::string original);

	std::string decode_message(std::string encoded);

	/* used for encoding */
	void build_encoding_table();
	
	/* used for decoding */
	void build_reverse_encoding_table();

	/* serialize tree into a file stream */
	void serialize(std::ofstream& file);

	/* for list tree command */
	void print();

	std::string get_encoding_for_character(char character);
private:

	/* represents a single node and its relationship with child */
	struct huffman_node
	{
		/* default constructor */
		huffman_node();

		/* destructor */
		~huffman_node()
		{
			/* free memory */
			delete left_child;
			delete right_child;
		}
		
		/* construct single node*/
		huffman_node(int number, char character_value);
		
		/* construct node with both left and right child */
		huffman_node(int number, huffman_node left, huffman_node right);

		/* copy constructor */
		huffman_node(const huffman_node& other) : huffman_node()
		{
			/* create this, by copying other */
			character = other.character;
			data = other.data;
			/* copy left and right child recursively (if you copy a node, all of its child will be copied)*/
			if (other.left_child)
				left_child = new huffman_node(*other.left_child);
			if (other.right_child)
				right_child = new huffman_node(*other.right_child);
		}

		/* assignment operator */
		huffman_node& operator=(huffman_node other) // if other is rvalue, move constructor is used, otherwise copy constructor is used
		{
			swap(*this, other); // make copy of "other" and swap with "this"

			return *this;
		}

		/* move constructor */
		huffman_node(huffman_node&& other) noexcept : huffman_node()
		{
			swap(*this, other);
		}

		/* swap, for copy and swap idiom */
		friend void swap(huffman_node& first, huffman_node& second) noexcept
		{
			using std::swap;

			/* swap members */
			swap(first.character, second.character);
			swap(first.data, second.data);
			swap(first.left_child, second.left_child);
			swap(first.right_child, second.right_child);
		}

		bool is_leaf() const
		{
			return !left_child && !right_child;
		}

		char character; // field is valid only if is_leaf() is true.
		int data;
		huffman_node* left_child;
		huffman_node* right_child;
	};

	huffman_node* root;
	std::unordered_map<char, std::string> encoding_table; // 'c' -> "100", 'b' -> "111" etc.
	std::unordered_map<std::string, char> reverse_encoding_table; // "111" -> 'b', "100" -> 'c' etc. 

	int reserved_character_count = 3;
	char reserved_chars[3] = { ' ', '\t', '|' };
	std::string escape_strings[3] = { "ss", "tt", "xx" };

	void build_encoding_table_internal(huffman_node* node, std::vector<bool> bit_storage);
	void serialize_internal(std::ofstream& file, huffman_node* node);
	void deserialize_internal(std::ifstream& file, huffman_node*& node); // receive pointer as ref so we can modify it
	void decode_message_internal(huffman_node* node, std::stringstream& output_stream, std::string done_encoded, std::string remaining_encoded);
	void print_internal(const std::string& prefix, huffman_node* node, bool is_right);
};
