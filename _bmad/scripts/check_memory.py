import sys
import re

# Validation mémoire STM32F407VG
# SRAM (Total : 192KB incluant CCM), CCM (16KB)
# Règles : Aucun buffer DMA en CCM (0x10000000 - 0x10003FFF)
# Budget max BSS + DATA : 192 KB

MAX_SRAM_KB = 192

def check_memory(map_file):
    with open(map_file, 'r') as f:
        content = f.read()

    # Détection CCM (Core Coupled Memory)
    dma_buffers = re.findall(r"0x(1000[0-9a-fA-F]+)\s+\d+\s+([\w\.]+)", content)
    for addr, symbol in dma_buffers:
        print(f"CRITICAL: DMA Buffer '{symbol}' found in CCM at address 0x{addr}")
        sys.exit(1)

    # Vérification budget BSS/DATA
    # Parsing simplifié du map file (section .bss et .data)
    bss_size = sum(int(size, 16) for size in re.findall(r"\.bss\s+0x[0-9a-fA-F]+\s+(0x[0-9a-fA-F]+)", content))
    data_size = sum(int(size, 16) for size in re.findall(r"\.data\s+0x[0-9a-fA-F]+\s+(0x[0-9a-fA-F]+)", content))
    
    total_kb = (bss_size + data_size) / 1024
    print(f"Total SRAM usage: {total_kb:.2f} KB")
    
    if total_kb > MAX_SRAM_KB:
        print(f"CRITICAL: Memory budget exceeded! Used {total_kb:.2f} KB, Max {MAX_SRAM_KB} KB.")
        sys.exit(1)

    print("Memory check passed: No DMA buffers in CCM and budget respected.")

if __name__ == "__main__":
    check_memory(sys.argv[1])
