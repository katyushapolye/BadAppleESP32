from PIL import Image
import numpy as np
import os
shape=(32,24)
def pad_and_convert_to_cpp(image_path):
    # Open the image
    image = Image.open(image_path)

    # Resize the image to 65x49

    resized_image = image.resize(shape)

    # Rotate the image by 90 degrees


    # Flip the image horizontally

    # Convert the resized image to grayscale
    grayscale_image = resized_image.convert("L")

    # Convert the grayscale image to a binary matrix
    binary_matrix = np.array(grayscale_image) > 128

    target_shape = (shape[1], shape[0])

    # Calculate padding amounts
    vertical_pad = target_shape[0] - binary_matrix.shape[0]
    horizontal_pad = target_shape[1] - binary_matrix.shape[1]

    # Calculate symmetric padding for the sides
    left_pad = horizontal_pad // 2
    right_pad = horizontal_pad - left_pad

    # Pad the matrix
    top_pad = (vertical_pad, 0)  # Padding on the top only
    side_pad = (left_pad, right_pad)  # Symmetrical padding on the sides
    padded_matrix = np.pad(binary_matrix, (top_pad, side_pad), mode='constant')

    def bitmap_to_cpp_matrix(bitmap):
        cpp_matrix = []
        for row in bitmap:
            cpp_row = []
            for i in range(0, len(row), 8):
                # Group every 8 bits into a byte
                byte = row[i:i+8]
                # Convert the byte to hex representation
                hex_byte = format(int(''.join(map(str, byte)), 2), '02x')
                # Append the hex representation to the row
                cpp_row.append('0x' + hex_byte)
            # Append the row to the matrix
            cpp_matrix.append(cpp_row)
        return cpp_matrix

    # Define characters for 0 and 1
    char_0 = '0'
    char_1 = '1'

    # Create an empty character matrix
    char_matrix = np.empty(padded_matrix.shape, dtype=str)

    # Transform binary matrix to char matrix
    for i in range(padded_matrix.shape[0]):
        for j in range(padded_matrix.shape[1]):
            if padded_matrix[i, j]:
                char_matrix[i, j] = char_1
            else:
                char_matrix[i, j] = char_0

    cpp_matrix = bitmap_to_cpp_matrix(char_matrix)

    return cpp_matrix

i = 0
with open("frame2.h", "w") as f:
    f.write("char frames[][{}][{}]".format(shape[1],int((shape[0])/8) ))
    f.write(" = {")
    while i < 6568:
        if (i % 3 == 0):
            image_path = "frames/frame{}.jpg".format(i)  # Replace with the path to your image
            cpp_matrix = pad_and_convert_to_cpp(image_path)
            f.write("{\n")
            for row in cpp_matrix:
                f.write('{' + ', '.join(row) + '},\n')
            f.write("},\n")
            i += 1
            print(i)
        else:
            i += 1
    f.write("};\n")


# Print the C++ matrix
for row in cpp_matrix:
    print('{' + ', '.join(row) + '},')
