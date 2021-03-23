#include <cassert>
#include <fstream>
#include <iostream>

#include "huffman_tree.h"
#include "input_helper.h"

constexpr auto tree_file_name = "tree.txt";

int main(int argc, char* argv[])
{
	//  supported formats:
	//  -i input_file.txt -encode
	//  -i input_file.txt -encode -s character
	//  -i input_file.txt -decode
	//  -l
	
	if (argc == 1)
	{
		std::cout << "No arguments detected!" << std::endl;
		return -1;
	}
	
	if (argc == 2)
	{
		/* only -l option is given */
		assert(std::string(argv[1]) == "-l");

		/* construct huffman tree from tree_file_name, then call print() on it */
		std::ifstream tree_file(tree_file_name);
		huffman_tree tree(tree_file);
		tree.print();
	}
	else
	{
		/* input file must be specified otherwise */
		assert(argc >= 4);
		assert(std::string(argv[1]) == "-i");

		std::string input_file_name = argv[2];
		std::string input_text; // remember, this can be either decoded or encoded text
		{
			/* at the end of this block, input_file will automatically be closed */
			std::ifstream input_file(input_file_name);
			std::getline(input_file, input_text); // we are sure to have one line input
			input_helper::erase_new_line_characters(input_text);
		}

		bool encoded = false;
		if(std::string(argv[3]) == "-encode")
		{
			auto freq_data = input_helper::get_character_frequency_map(input_text);
			if (freq_data.empty())
			{
				std::cout << "Input file is empty, exiting!" << std::endl;
				return -1;
			}
			
			huffman_tree tree(freq_data);
			{
				/* output serialized tree file so we can use it for decoding later */
				std::ofstream tree_file(tree_file_name);
				tree.serialize(tree_file);
			}

			std::cout << "Encoded Message: " << tree.encode_message(input_text) << std::endl;
			encoded = true;
		}
		else
		{
			assert(std::string(argv[3]) == "-decode");
			/* load saved tree file */
			std::ifstream tree_file(tree_file_name);
			huffman_tree tree(tree_file);

			std::cout << "Decoded Message: " << tree.decode_message(input_text) << std::endl;
		}
		
		if (argc == 6)
		{
			/* optional -s given, it can only be used with -encode setting */
			assert(encoded);
			char character = std::string(argv[5])[0];
			std::ifstream tree_file(tree_file_name);
			huffman_tree tree(tree_file);
			std::cout << "Encoding for character " << character << " is: " << tree.get_encoding_for_character(character) << std::endl;
		}
	}
	
	return 0;
}

