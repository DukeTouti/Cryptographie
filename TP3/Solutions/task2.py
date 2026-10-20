#!/usr/bin/env python3
"""
Task 2 - Compute SHA256 Padding
Block size = 64 bytes.
Padding = \x80 + zeros + 8-byte big-endian bit-length.
"""

NAME = "MohammedTahaHathouti"
UID  = "1001"
KEY  = "123456"

message = f"{KEY}:myname={NAME}&uid={UID}&lstcmd=1"
msg_len = len(message.encode())
print(f"Message        : {message}")
print(f"Message length : {msg_len} bytes")

block_size = 64
# bytes needed after message to reach next multiple of 64
padding_len = block_size - (msg_len % block_size)
if padding_len == 0:
    padding_len = block_size

bit_length = msg_len * 8

# build padding bytes
padding = b'\x80'
padding += b'\x00' * (padding_len - 1 - 8)   # zeros
padding += bit_length.to_bytes(8, 'big')        # 8-byte big-endian length

assert len(padding) == padding_len, f"padding length mismatch: {len(padding)} != {padding_len}"

# URL-encoded form (\xNN → %NN)
url_padding = ''.join(f'%{b:02x}' for b in padding)

print(f"Padding length : {padding_len} bytes")
print(f"Bit length     : {bit_length} = {hex(bit_length)}")
print(f"Padding (hex)  : {padding.hex()}")
print(f"Padding (URL)  : {url_padding}")
print()
print("Full padded message (hex):")
print((message.encode() + padding).hex())
