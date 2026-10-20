#!/usr/bin/env python3
"""
Task 1 - Send Request to List Files
MAC = SHA256(key:myname=<name>&uid=<uid>&lstcmd=1)
"""

import hashlib

NAME = "MohammedTahaHathouti"
UID  = "1001"
KEY  = "123456"

# --- lstcmd only ---
message_lst = f"myname={NAME}&uid={UID}&lstcmd=1"
payload_lst = f"{KEY}:{message_lst}"
mac_lst = hashlib.sha256(payload_lst.encode()).hexdigest()

print("=== Task 1a: List Files ===")
print(f"Payload : {payload_lst}")
print(f"MAC     : {mac_lst}")
print(f"URL     : http://www.seedlab-hashlen.com/?{message_lst}&mac={mac_lst}")

# --- lstcmd + download ---
message_dl = f"myname={NAME}&uid={UID}&lstcmd=1&download=secret.txt"
payload_dl = f"{KEY}:{message_dl}"
mac_dl = hashlib.sha256(payload_dl.encode()).hexdigest()

print("\n=== Task 1b: Download secret.txt ===")
print(f"Payload : {payload_dl}")
print(f"MAC     : {mac_dl}")
print(f"URL     : http://www.seedlab-hashlen.com/?{message_dl}&mac={mac_dl}")
