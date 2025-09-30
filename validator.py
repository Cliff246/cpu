import struct
import sys

HEADER_DWORDS = 6  # first 6 × 64-bit words

def decode_inst(word):
    path   = (word >> 28) & 0xF
    subop  = (word >> 20) & 0xFF
    rd     = (word >> 15) & 0x1F
    rs1    = (word >> 10) & 0x1F
    rs2    = (word >> 5)  & 0x1F
    aux    = (word >> 2)  & 0x7
    immf   = word & 0x3
    return (path, subop, rd, rs1, rs2, aux, immf)

def validate_bin(path):
    with open(path, "rb") as f:
        data = f.read()

    # --- Header ---
    header = [struct.unpack_from("<Q", data, i*8)[0] for i in range(HEADER_DWORDS)]
    print("Header (6×64-bit dwords):")
    for i, h in enumerate(header):
        print(f"  [{i}] = {h}")

    # Assign header fields to named variables
    code_ptr, code_len, inst_ptr, inst_len, imm_ptr, imm_len = header

    filesize = len(data)

    # Sanity checks for regions within file size
    if not (0 <= code_ptr <= filesize) or not (0 <= code_ptr + code_len <= filesize):
        print(f"Error: code table region [{code_ptr}, {code_ptr + code_len}) out of file bounds")
        return
    if not (0 <= inst_ptr <= filesize) or not (0 <= inst_ptr + inst_len <= filesize):
        print(f"Error: instruction region [{inst_ptr}, {inst_ptr + inst_len}) out of file bounds")
        return
    if not (0 <= imm_ptr <= filesize) or not (0 <= imm_ptr + imm_len <= filesize):
        print(f"Error: immediate region [{imm_ptr}, {imm_ptr + imm_len}) out of file bounds")
        return

    # Check for overlaps
    regions = [
        ("code", code_ptr, code_ptr + code_len),
        ("inst", inst_ptr, inst_ptr + inst_len),
        ("imm", imm_ptr, imm_ptr + imm_len)
    ]
    for i in range(len(regions)):
        for j in range(i+1, len(regions)):
            name_i, start_i, end_i = regions[i]
            name_j, start_j, end_j = regions[j]
            if max(start_i, start_j) < min(end_i, end_j):
                print(f"Error: {name_i} region [{start_i}, {end_i}) overlaps with {name_j} region [{start_j}, {end_j})")
                return

    # --- Instruction stream ---
    print(f"\nDecoding {inst_len} instructions (from offset {inst_ptr}):")
    immf_nonzero_count = 0
    for i in range(inst_len):
        word, = struct.unpack_from("<I", data, inst_ptr + i*8)
        fields = decode_inst(word)
        print(f"  inst[{i:03}] 0x{word:08X} path={fields[0]} subop={fields[1]} "
              f"rd={fields[2]} rs1={fields[3]} rs2={fields[4]} aux={fields[5]} immf={fields[6]}")
        if fields[6] != 0:
            immf_nonzero_count += 1

    if immf_nonzero_count != imm_len:
        print(f"Error: immf != 0 count ({immf_nonzero_count}) does not match imm_len ({imm_len})")

    # --- Immediate table ---
    print(f"\nImmediate table ({imm_len} entries at offset {imm_ptr}):")
    for i in range(imm_len):
        imm_val, = struct.unpack_from("<Q", data, imm_ptr + i*8)
        print(f"  imm[{i:03}] = 0x{imm_val:016X}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <binfile>")
        sys.exit(1)
    validate_bin(sys.argv[1])
