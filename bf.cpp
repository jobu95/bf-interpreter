/*
 * Brainfuck interpreter. Reads instructions from stdin, writes output to
 * stdout. On error, writes an error message to stderr and exits.
 *
 * Usage:
 * ./bf < brainfuckProgram.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <vector>

typedef unsigned char byte;

bool getInput(std::vector<byte> &input, int &ip, FILE *in, byte &instr);

int main()
{
  FILE *in = stdin;
  if (in == NULL) {
    fprintf(stderr, "Failed to open stdin. Exiting...\n");
    exit(1);
  }

  std::vector<byte> input;
  byte memory[30000], instr;
  bzero(memory, sizeof(memory));
  int ip = 0; // instruction pointer
  int mem_ptr = 0;

  while(getInput(input, ip, in, instr)) {
    switch(instr) {
      case('+'):
        memory[mem_ptr]++;
        break;
      case('-'):
        memory[mem_ptr]--;
        break;
      case('>'):
        mem_ptr++;
        if (mem_ptr >= sizeof(memory)) {
          mem_ptr = 0;
        }
        break;
      case('<'):
        mem_ptr--;
        if (mem_ptr < 0) {
          mem_ptr = sizeof(memory) - 1;
        }
        break;
      case('.'):
        printf("%c", memory[mem_ptr]);
        break;
      case(','):
        getInput(input, ip, in, memory[mem_ptr]);
        break;
      case('['):
        // if current cell is 0, skip past matching ']' in input
        if (memory[mem_ptr] == 0) {
          int scope_level = 1;
          while (getInput(input, ip, in, instr)) {
            if (instr == '[') {
              scope_level++;
            } else if (instr == ']') {
              scope_level--;
            }
            if (scope_level == 0) {
              break;
            }
          }
          if (scope_level != 0) {
            printf("Unmatched opening bracket. Exiting...\n");
            exit(1);
          }
        }
        break;
      case(']'):
        // if current cell is nonzero, jump back to instr after matching '[' in
        // input
        if (memory[mem_ptr] != 0) {
          int scope_level = 1;
          ip--; // ip is currently set to the instr past the ']', so move it
              // back to the bracket
          while (ip > 0 && scope_level > 0) {
            ip--;
            if (input[ip] == '[') {
              scope_level--;
            } else if (input[ip] == ']') {
              scope_level++;
            }
            if (scope_level == 0) {
              break;
            }
          }
          if (scope_level != 0) {
            printf("Unmatched closing bracket. Exiting...\n");
            exit(1);
          }
        }
        break;
      case('#'): // comment. skip past instructions till newline
        while(getInput(input, ip, in, instr)) {
          if (instr == '\n') {
            break;
          }
        }
        break;
      default:
        // ignore unrecognized instructions
        break;
    }
  }

  return 0;
}

bool getInput(std::vector<byte> &input, int &ip, FILE *in, byte &instr)
{
  if (input.size() == ip) {
    if (!in) {
      return false;
    }
    if (fread(&instr, 1, 1, in) != 1) {
      return false;
    }
    input.push_back(instr);
    ip++;
    return true;
  } else if (input.size() > ip) {
    instr = input[ip++];
    return true;
  } else {
    fprintf(stderr, "This should never happen. Exiting...\n");
    exit(1);
  }
}
