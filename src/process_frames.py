# Copyright (c) 2025 Chardson Coelho
# Licensed under the MIT License. See LICENSE file for details.

from natsort import natsorted
from PIL import Image
import os

SCREEN_HEIGHT = 56
SCREEN_WIDTH = 200

ASCII_BRIGHTNESS = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`\'. " 

s_frames = os.listdir("./frames")
s_frames = natsorted(s_frames)

file = open("./assets/badapple.dat", "wb")

ticks = 0
for s_frame in s_frames:
    img_frame = Image.open(f"./frames/{s_frame}")

    img_frame = img_frame.resize((SCREEN_WIDTH, SCREEN_HEIGHT))
    img_frame = img_frame.convert('L')

    char_buff = []

    frame_pixels = img_frame.load()
    for x in range(SCREEN_WIDTH):
        for y in range(SCREEN_HEIGHT):
            pixel_value = frame_pixels[x, y]
            index = int(pixel_value * 69 / 255)
            ch = ASCII_BRIGHTNESS[69 - index]
            char_buff.append(ch)
    
    byte_data = ''.join(char_buff).encode("utf-8")

    file.write(byte_data)

    ticks += 1
    if ticks % 1000 == 0:
        print(f"Processed frame {s_frame}")

file.close()