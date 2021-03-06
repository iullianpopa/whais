/******************************************************************************
WHAIS - An advanced database system
Copyright(C) 2014-2018  Iulian Popa

Address: Str Olimp nr. 6
         Pantelimon Ilfov,
         Romania
Phone:   +40721939650
e-mail:  popaiulian@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef OPCODES_H_
#define OPCODES_H_

#include "compiler/wopcodes.h"
#include "utils/woutstream.h"
#include "vardecl.h"


extern const enum W_OPCODE inc_op[T_END_OF_TYPES];
extern const enum W_OPCODE dec_op[T_END_OF_TYPES];
extern const enum W_OPCODE not_op[T_END_OF_TYPES];
extern const enum W_OPCODE add_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE sub_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE mul_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE div_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE mod_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE less_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE less_eq_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE greater_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE greater_eq_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE equals_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE not_equals_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE or_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE xor_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE and_op[T_END_OF_TYPES][T_END_OF_TYPES];
extern const enum W_OPCODE store_op[T_END_OF_TYPES][T_END_OF_TYPES];


struct WOutputStream*
encode_opcode(struct WOutputStream  *stream,
              const enum W_OPCODE    opcode);

enum W_OPCODE
decode_opcode(const uint8_t *instrs);

uint_t
opcode_bytes(const enum W_OPCODE);

#endif /* OPCODES_H_ */
