import heapq
from collections import Counter, defaultdict

def binary_to_chars(binary_string):
    # Split the binary string into groups of 8
    chunks = [binary_string[i:i+8] for i in range(0, len(binary_string), 8)]
    #print("Chunks:", chunks)
    
    # Convert each chunk to its corresponding ASCII character
    chars = [chr(int(chunk, 2)) for chunk in chunks]
    #print("Chars:", chars)
    
    return chars,chunks

def binary_file_to_string(file_path):
    try:
        with open(file_path, 'rb') as file:
            # Read the binary data from the file
            binary_data = file.read()

            # Convert each byte to its binary representation and concatenate them
            binary_string = ''.join(format(byte, '08b') for byte in binary_data)

            return binary_string
    except FileNotFoundError:
        return "File not found."

def write_chars_to_file(chars, filename):
    with open(filename, 'w',encoding="latin-1") as file:
        for char in chars:
            file.write(char)

class Node:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

def build_huffman_tree(freq_map):
    priority_queue = [Node(char, freq) for char, freq in freq_map.items()]
    heapq.heapify(priority_queue)
    
    while len(priority_queue) > 1:
        left = heapq.heappop(priority_queue)
        right = heapq.heappop(priority_queue)
        
        merged_node = Node(None, left.freq + right.freq)
        merged_node.left = left
        merged_node.right = right
        
        heapq.heappush(priority_queue, merged_node)
    
    return priority_queue[0]

def build_huffman_code_table(root):
    code_table = {}
    
    def dfs(node, code):
        if node:
            if node.char is not None:
                code_table[node.char] = code
            dfs(node.left, code + '0')
            dfs(node.right, code + '1')
    
    dfs(root, '')
    return code_table

def huffman_compress(text):
    freq_map = Counter(text)
    root = build_huffman_tree(freq_map)
    code_table = build_huffman_code_table(root)
    
    compressed_text = ''.join(code_table[char] for char in text)


    if len(compressed_text) % 8 != 0:
        print("Adding padding")
        compressed_text += '0' * (8 - (len(compressed_text) % 8))

    return compressed_text, code_table


def print_frequency_and_huffman(text):
    freq_map = Counter(text)
    print("Character frequencies:")
    for char, freq in freq_map.items():
        hex_representation = format(char, '02x')
        print(f"0x{hex_representation}: {freq}")
    
    compressed_text, code_table = huffman_compress(text)
    print("\nHuffman Codes:")

    print("char characters[] = {")
    for char, code in code_table.items():
        hex_representation = format(char, '02x')
        print(f"0x{hex_representation},")
    print("}")

    print("char* huffmancodes[] = {")
    for char, code in code_table.items():
        hex_representation = format(char, '02x')
        print("\"{}\",".format(code))
    print("}")

    

    #print("\nCompressed Text:")

    print("Compression lenght in bits: {}".format(len(compressed_text)))

    print("Compression rate: {}".format((((len(compressed_text))/8)/len(text))**-1))
    return compressed_text




def first_difference_index(str1, str2):
    min_len = min(len(str1), len(str2))
    for i in range(min_len):
        if str1[i] != str2[i]:
            return i
    # If the loop completes without finding a difference, return the length of the shortest string
    return min_len

def write_bitstream_to_file(bitstream, filename):
    with open(filename, 'wb') as file:
        byte = 0
        bit_count = 0
        for bit_char in bitstream:
            if bit_char == '1':
                byte = (byte << 1) | 1
            elif bit_char == '0':
                byte = byte << 1
            else:
                raise ValueError("Invalid bit character found in bitstream")
            bit_count += 1
            if bit_count == 8:
                file.write(bytes([byte]))
                byte = 0
                bit_count = 0
        if bit_count > 0:
            byte <<= (8 - bit_count)
            file.write(bytes([byte]))

str1 = "apple"
str2 = "apply"

index = first_difference_index(str1, str2)
if index < min(len(str1), len(str2)):
    print("First difference at index:", index)
else:
    print("No differences found within the common length of the strings.")


if __name__ == "__main__":
    with open("frame2.raw", "rb") as file:
        text2 = file.read()
    text = print_frequency_and_huffman(text2)

    #print("Text:  " + text)

    chars = binary_to_chars(text)[0]
    chars_str = ''.join(chars)


    if("".join(binary_to_chars(text)[1]) == text):
        print("EQUAL IN BIN TO CHAR")
        
    #print("Converted chars " + chars_str)
    #write_chars_to_file(chars,"compressed.huff")
    write_bitstream_to_file(text,"compressed2.bin")


    stringRead = binary_file_to_string("compressed2.bin")

    #print("Original compressed " + text)
    #print("Saved on file " +stringRead)

    

    if(stringRead == text):
        print("Equal")
        print("Read from file size: {}".format(len(stringRead)))
        print("Compressed size: {}".format(len(text)))
    else:
        print("Not equal")
        print("Read from file size: {}".format(len(stringRead)))
        print("Compressed size: {}".format(len(text)))




