#include <cstdint>
#include <instructions.hh>
#include <memory.hh>

using namespace instrs;
using namespace mem;

// Assuming Little Endian
// LB 0b000
// Load Byte
template<>
void instrs::execute_load<0b000>(mem::memory& mem, processor & proc, mem::address_t addr, uint8_t rd) {
  proc.write_reg(rd, sign_extend<uint32_t, sizeof(uint8_t)*8>(mem.read<uint8_t>(addr)));
}

// LH 0b001
// Load Halfword
template<>
void instrs::execute_load<0b001>(mem::memory& mem, processor & proc, mem::address_t addr, uint8_t rd) {
  assert((addr & 0b1) == 0); // ensure 2 byte alignment
  proc.write_reg(rd, sign_extend<uint32_t, sizeof(uint16_t)*8>(mem.read<uint16_t>(addr)));
}
// LWU 0b010
// Load Word
template<>
void instrs::execute_load<0b010>(mem::memory& mem, processor & proc, mem::address_t addr, uint8_t rd) {
  assert((addr & 0b11) == 0); // ensure 4 byte alignment
  proc.write_reg(rd, mem.read<uint32_t>(addr));
}

// LBU 0b100
// Load Byte Unsigned
template<>
void instrs::execute_load<0b100>(mem::memory& mem, processor & proc, mem::address_t addr, uint8_t rd) {
  proc.write_reg(rd, mem.read<uint8_t>(addr));
}



// LHU 0b101
// Load Halfword Unsigned
template<>
void instrs::execute_load<0b101>(mem::memory& mem, processor & proc, mem::address_t addr, uint8_t rd) {
  assert((addr & 0b1) == 0); // ensure 2 byte alignment
  proc.write_reg(rd, mem.read<uint16_t>(addr));
}

uint32_t instrs::load(mem::memory& mem, processor& proc, uint32_t bitstream) {

  // Decode the instruction
  i_instruction ii(bitstream);

  // Calculate the source address
  address_t addr = proc.read_reg(ii.rs1()) + ii.imm();

  // Execute the instruction
  switch (ii.funct3()) {
    case 0b000: execute_load<0b000>(mem, proc, addr, ii.rd()); break;
    case 0b001: execute_load<0b001>(mem, proc, addr, ii.rd()); break;
    case 0b010: execute_load<0b010>(mem, proc, addr, ii.rd()); break;
    case 0b100: execute_load<0b100>(mem, proc, addr, ii.rd()); break;
    case 0b101: execute_load<0b101>(mem, proc, addr, ii.rd()); break;
  }
  // Return the next PC
  return proc.next_pc();
}

// SB 0b000
// Store Byte
template<>
void instrs::execute_store<0b000>(mem::memory& mem, processor & proc, mem::address_t addr, uint8_t rs2) {
  mem.write<uint8_t>(addr, static_cast<uint8_t>(proc.read_reg(rs2)));
}

// SH 0b001
// Store Halfword
template<>
void instrs::execute_store<0b001>(mem::memory& mem, processor & proc, mem::address_t addr, uint8_t rs2) {
  assert((addr & 0b1) == 0); // ensure 2 byte alignment
  mem.write<uint16_t>(addr, static_cast<uint16_t>(proc.read_reg(rs2)));
}

// SW 0b010
// Store Word
template<>
void instrs::execute_store<0b010>(mem::memory& mem, processor & proc, mem::address_t addr, uint8_t rs2) {
  assert((addr & 0b11) == 0); // ensure 4 byte alignment
  mem.write<uint32_t>(addr, proc.read_reg(rs2));
}

uint32_t instrs::store(mem::memory& mem, processor& proc, uint32_t bitstream) {
  // Decode the instruction
  s_instruction si(bitstream);

  // Calculate the source address
  address_t addr = proc.read_reg(si.rs1()) + si.imm();

  // Execute the instruction
  switch (si.funct3()) {
    case 0b000: execute_store<0b000>(mem, proc, addr, si.rs2()); break;
    case 0b001: execute_store<0b001>(mem, proc, addr, si.rs2()); break;
    case 0b010: execute_store<0b010>(mem, proc, addr, si.rs2()); break;
  }
  // Return the next PC
  return proc.next_pc();
}

// Arithmetic Inmediate
uint32_t instrs::alui(mem::memory &, processor &proc, uint32_t bitstream) {
  i_instruction ii(bitstream);

  uint32_t val = proc.read_reg(ii.rs1());

  switch (ii.funct3()) {
    case 0b0000000: // ADDI
      proc.write_reg(ii.rd(), val + ii.imm());
      break;
    case 0b0000001: // SLLI
      // Desplazamiento lógico a la izquierda (SLLI) determinado por los 5 bits menos significativos de imm
      proc.write_reg(ii.rd(), val << (ii.imm() & 0x1F)); 
      break;
  }
  return proc.next_pc();
}

// Arithmetic Register
uint32_t instrs::alur(mem::memory &, processor &proc, uint32_t bitstream) {
  r_instruction ri(bitstream);

  uint32_t val1 = proc.read_reg(ri.rs1());
  uint32_t val2 = proc.read_reg(ri.rs2());

  switch (ri.funct3()) {
    case 0b000: // ADD o MUL
      if (ri.funct7() == 0b0000000) { // ADD
        proc.write_reg(ri.rd(), val1 + val2);
      } else if (ri.funct7() == 0b0000001) { // MUL
        proc.write_reg(ri.rd(), val1 * val2);
      }
      break;
  }
  return proc.next_pc();
}

// Load Upper Immediate
uint32_t instrs::lui(mem::memory &, processor &proc, uint32_t bitstream) {
  u_instruction ui(bitstream);

  proc.write_reg(ui.rd(), ui.imm());
  return proc.next_pc();
}

// Jump and Link
uint32_t instrs::jal(mem::memory &, processor &proc, uint32_t bitstream) {
  j_instruction ji(bitstream);

  // Save the return address
  uint32_t current_pc = proc.read_pc(); //PC actual
  proc.write_reg(1, current_pc + 4);

  // Calculate the jump address
  address_t addr = current_pc + ji.imm(); //PC actual + offset

  // Return the jump address
  return addr;
}

// Jump and Link Register
uint32_t instrs::jalr(mem::memory &, processor &proc, uint32_t bitstream) {
  i_instruction ii(bitstream);

  // Save the return address
  uint32_t current_pc = proc.read_pc();
  proc.write_reg(ii.rd(), current_pc + 4);

  // Calculate the jump address
  address_t addr = (proc.read_reg(ii.rs1()) + ii.imm()) & ~1; //PC actual + offset alineado a una dirección par

  // Return the jump address
  return addr;
}

// Conditional Branch
uint32_t instrs::condbranch(mem::memory &, processor & proc, uint32_t bitstream) {
  b_instruction bi(bitstream);

  uint32_t current_pc = proc.read_pc();
  uint32_t val1 = proc.read_reg(bi.rs1());
  uint32_t val2 = proc.read_reg(bi.rs2());

  bool take_branch = false;
  switch (bi.funct3()) {
    case 0b000: // BEQ
      take_branch = (val1 == val2);
      break;
    case 0b001: // BNE
      take_branch = (val1 != val2);
      break;
    case 0b100: // BLT
      take_branch = ((int32_t)val1 < (int32_t)val2);
      break;
    case 0b101: // BGE
      take_branch = ((int32_t)val1 >= (int32_t)val2);
      break;
    case 0b110: // BLTU
      take_branch = (val1 < val2);
      break;
    case 0b111: // BGEU
      take_branch = (val1 >= val2);
      break;
  }

  // Si se cumple la condición, se suma el offset (imm) al siguiente PC
  address_t addr = take_branch ? (current_pc + bi.imm()) : proc.next_pc();
  return addr;
}

