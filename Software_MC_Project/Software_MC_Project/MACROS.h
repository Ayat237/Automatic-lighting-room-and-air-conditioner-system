#ifndef MACROS_H_
#define MACROS_H_

#define SET_BIT(REG,BIT) (REG|=(1<<BIT)) //Bit = 1 "output"
#define RESET_BIT(REG,BIT) (REG &= (~(1<<BIT))) //Bit = 0 "Input"
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))
#define READ_BIT(REG,BIT) (REG&(1<<BIT))
#define SET_REG(REG) (REG = 0xFF)

#endif