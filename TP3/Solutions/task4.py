#!/usr/bin/env python3
"""
Task 4 - HMAC-based MAC (attack mitigation)
Compute HMAC-SHA256 to replace the vulnerable SHA256(key||message) scheme.
"""

import hmac
import hashlib

NAME = "MohammedTahaHathouti"
UID  = "1001"
KEY  = "123456"

# --- Task 4a: lstcmd only ---
message = f"myname={NAME}&uid={UID}&lstcmd=1"
mac = hmac.new(
    bytearray(KEY.encode('utf-8')),
    msg=message.encode('utf-8', 'surrogateescape'),
    digestmod=hashlib.sha256
).hexdigest()

print("=== Task 4: HMAC-based request ===")
print(f"Message : {message}")
print(f"HMAC    : {mac}")
print(f"URL     : http://www.seedlab-hashlen.com/?{message}&mac={mac}")

# --- Task 4b: show the length extension attack now fails ---
NAME2 = "MohammedTahaHathouti"
msg_len = len(f"123456:{message}".encode())
pad_len = 64 - (msg_len % 64) or 64
bit_len = msg_len * 8
padding = b'\x80' + b'\x00' * (pad_len - 9) + bit_len.to_bytes(8, 'big')
url_padding = ''.join(f'%{b:02x}' for b in padding)

print("\n=== Attack attempt (should FAIL with HMAC) ===")
print(f"Forged URL would be:")
print(f"http://www.seedlab-hashlen.com/?myname={NAME2}&uid={UID}&lstcmd=1{url_padding}&download=secret.txt&mac=<forged>")
print("\nWhy it fails: HMAC uses key in both inner and outer hash.")
print("The attacker cannot extend the hash without knowing the key.")
