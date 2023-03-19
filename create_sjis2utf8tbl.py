#!/usr/bin/python3
import json

with open('library.json') as f:
    library_json = json.load(f)

with open('sjis2utf8.tbl', 'wb') as f:
    header = bytes(f"""
    S_JIS to UTF-8  Conversion table v{library_json['version']}
    Licensed under {library_json['license']}
    URL: {library_json['homepage']}
    """.strip(), 'utf-8')
    f.write(header)
    f.write(b'\x00' * (0xB0 - len(header)))
    for sjis_code in range(0x8140, 0xEAAF, 1):
        try:
            uni_code = ord(sjis_code.to_bytes(2, byteorder='big').decode('cp932'))
            f.write(uni_code.to_bytes(2, byteorder='little'))
        except (UnicodeDecodeError, TypeError):
            f.write(b'\x00\x00')

