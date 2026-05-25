import sys
import re

# Validation mémoire STM32F407VG
# SRAM1 (112KB), SRAM2 (64KB), CCM (16KB)
# Règles : Aucun buffer DMA en CCM (0x10000000 - 0x10003FFF)

def check_memory(map_file):
    with open(map_file, 'r') as f:
        content = f.read()

    # Détection CCM (Core Coupled Memory)
    ccm_range = range(0x10000000, 0x10004000)
    
    # Simple pattern pour trouver les symboles DMA dans le map
    dma_buffers = re.findall(r"0x(1000[0-9a-fA-F]+)\s+\d+\s+([\w\.]+)", content)
    
    for addr, symbol in dma_buffers:
        print(f"CRITICAL: DMA Buffer '{symbol}' found in CCM at address 0x{addr}")
        sys.exit(1)

    print("Memory check passed: No DMA buffers in CCM.")

if __name__ == "__main__":
    check_memory(sys.argv[1])
