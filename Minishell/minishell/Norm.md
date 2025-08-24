# 42 School Norm - Coding Standards Guide (Version 4.1)

A comprehensive guide to the 42 School programming standard that defines rules for writing clean, readable, and maintainable C code.

## Table of Contents

- [Overview](#overview)
- [Why Follow the Norm?](#why-follow-the-norm)
- [Quick Reference](#quick-reference)
- [Naming Conventions](#naming-conventions)
- [Formatting Rules](#formatting-rules)
- [Functions](#functions)
- [Data Structures](#data-structures)
- [Headers](#headers)
- [File Requirements](#file-requirements)
- [Macros and Preprocessors](#macros-and-preprocessors)
- [Forbidden Constructs](#forbidden-constructs)
- [Comments](#comments)
- [Makefile Rules](#makefile-rules)
- [Validation Tools](#validation-tools)
- [Common Mistakes](#common-mistakes)

## Overview

The 42 Norm is a programming standard that ensures code consistency, readability, and maintainability across all C projects in the 42 curriculum. It emphasizes simplicity, clarity, and best practices for beginning programmers.

### Key Principles
- **Sequencing**: Clear, linear execution flow
- **Readability**: Consistent formatting and naming
- **Maintainability**: Simple, understandable code structure
- **Community**: Recognizable coding style

## Why Follow the Norm?

### Educational Benefits
- **Sequencing**: Forces decomposition of complex tasks into simple, sequential instructions
- **Clarity**: Promotes understanding through consistent structure
- **Collaboration**: Enables efficient peer learning and code reviews
- **Professional Preparation**: Instills good coding habits for industry work

### Long-term Advantages
- Easier debugging and maintenance
- Faster code comprehension
- Reduced technical debt
- Professional code quality

## Quick Reference

### Essential Rules Checklist
- [ ] Functions ≤ 25 lines
- [ ] Lines ≤ 80 characters
- [ ] Max 4 function parameters
- [ ] Max 5 variables per function
- [ ] Max 5 functions per file
- [ ] Use 4-space tabs for indentation
- [ ] Include 42 header in all files
- [ ] Follow naming conventions
- [ ] No forbidden constructs (for, do-while, ternary, etc.)

## Naming Conventions

### Prefixes
- **Structures**: `s_` (e.g., `struct s_list`)
- **Typedefs**: `t_` (e.g., `typedef struct s_list t_list`)
- **Unions**: `u_` (e.g., `union u_data`)
- **Enums**: `e_` (e.g., `enum e_status`)
- **Globals**: `g_` (e.g., `int g_counter`)

### General Rules
- **Snake case only**: `my_function`, `file_name.c`
- **Lowercase + digits + underscores**: No uppercase letters
- **Descriptive names**: Avoid abbreviations like `i`, `tmp`, `ptr`
- **English words**: Clear, readable identifiers
- **ASCII only**: Except in string literals

### Examples
```c
// ✅ Good
int calculate_average(int *numbers, int count);
char *get_user_name(void);
typedef struct s_student t_student;

// ❌ Bad
int calcAvg(int *n, int c);
char *getUserName(void);
typedef struct Student Student;
```

## Formatting Rules

### Line and Function Limits
- **Function length**: Maximum 25 lines (excluding braces)
- **Line length**: Maximum 80 characters (including comments)
- **Functions per file**: Maximum 5 function definitions

### Indentation and Spacing
- **Indentation**: 4-character tabs (not spaces)
- **Braces**: On separate lines (except struct/enum/union declarations)
- **Empty lines**: Must be completely empty (no spaces/tabs)
- **Function separation**: One empty line between functions

### Code Structure
```c
// ✅ Correct formatting
int	my_function(int param1, char *param2)
{
	int		local_var;
	char	*result;

	local_var = 42;
	result = process_data(param1, param2);
	return (local_var);
}
```

### Variable Declarations
- **Beginning of function**: All declarations at the top
- **Aligned columns**: Variable names aligned within scope
- **Pointer asterisks**: Attached to variable name (`int *ptr`)
- **One per line**: No multiple declarations (`int a, b;` ❌)
- **Separate initialization**: No inline initialization in functions

```c
// ✅ Correct variable declarations
int	my_function(void)
{
	int		counter;
	char	*buffer;
	size_t	length;

	counter = 0;
	buffer = malloc(100);
	length = strlen(buffer);
	return (counter);
}
```

## Functions

### Parameter Rules
- **Maximum 4 parameters**: Keep function signatures simple
- **Void for no parameters**: `int function(void)`
- **Named parameters**: All parameters must be named in prototypes
- **Return values**: Always use parentheses `return (value);`

### Function Declaration Format
```c
// ✅ Correct function format
int	calculate_sum(int a, int b, int c, int d)
{
	int	result;

	result = a + b + c + d;
	return (result);
}

void	print_message(void)
{
	printf("Hello, World!\n");
	return ;
}
```

### Variable Limits
- **Maximum 5 variables** per function
- Includes all local variables, not parameters
- Forces function decomposition for complex operations

## Data Structures

### Structure Declaration
```c
// ✅ Correct structure format
typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
```

### Rules
- **Space after keywords**: `struct s_name`, `enum e_name`
- **Regular indentation**: Inside braces
- **Aligned names**: Structure members aligned
- **No .c declarations**: Structures only in header files

## Headers

### Header Structure
```c
#ifndef PROJECT_H
# define PROJECT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define BUFFER_SIZE 42
# define SUCCESS 0
# define ERROR -1

typedef struct s_data	t_data;

int		my_function(char *str);
void	*my_malloc(size_t size);

#endif
```

### Header Rules
- **Include guard**: `#ifndef PROJECT_H`
- **System includes first**: Then project includes
- **No unused includes**: Only necessary headers
- **No .c includes**: Never include .c files
- **Prototypes only**: Functions, defines, typedefs, declarations

### 42 Header Requirement
Every `.c` and `.h` file must start with the standard 42 header:
```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by username          #+#    #+#             */
/*   Updated: 2023/01/01 00:00:00 by username         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
```

## File Requirements

### File Organization
- **Maximum 5 functions** per `.c` file
- **No .c includes**: Never include `.c` files in other files
- **Logical grouping**: Related functions in same file
- **Clear filenames**: Descriptive, snake_case names

### File Naming
```
✅ Good filenames:
- string_utils.c
- memory_management.c
- list_operations.c

❌ Bad filenames:
- utils.c
- helpers.c
- functions.c
```

## Macros and Preprocessors

### Define Rules
- **Constants only**: Use #define for literal values
- **UPPERCASE names**: All macro names
- **No code obfuscation**: Don't bypass norm with macros
- **No multiline macros**: Single line only
- **Global scope only**: No preprocessor in functions

```c
// ✅ Correct macro usage
# define PI 3.14159
# define BUFFER_SIZE 1024
# define SUCCESS 0
# define ERROR -1

// ❌ Incorrect usage
# define MAX(a, b) ((a) > (b) ? (a) : (b))  // Multiline forbidden
# define LOOP for(int i = 0; i < 10; i++)    // Code obfuscation
```

### Conditional Compilation
```c
# ifdef DEBUG
	printf("Debug mode enabled\n");
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif
```

## Forbidden Constructs

### Completely Forbidden
- `for` loops
- `do...while` loops
- `switch` statements
- `case` statements
- `goto` statements
- Ternary operators (`condition ? true : false`)
- Variable Length Arrays (VLAs)
- Implicit type declarations

### Alternatives
```c
// ❌ Forbidden
for (int i = 0; i < 10; i++)
	printf("%d\n", i);

// ✅ Use while instead
int i = 0;
while (i < 10)
{
	printf("%d\n", i);
	i++;
}

// ❌ Forbidden
result = (a > b) ? a : b;

// ✅ Use if-else
if (a > b)
	result = a;
else
	result = b;
```

## Comments

### Comment Rules
- **No function body comments**: Comments outside function bodies only
- **End of line or separate line**: Two positions allowed
- **English language**: Clear, professional language
- **Useful content**: Explain "why", not "what"
- **No code justification**: Don't use comments to excuse bad code

```c
// ✅ Good comments
#include "minishell.h"  // Main project header

/*
** This function implements the quicksort algorithm
** for integer arrays with optimized pivot selection
*/
void	quick_sort(int *array, int low, int high)
{
	int	pivot_index;

	if (low < high)
	{
		pivot_index = partition(array, low, high);
		quick_sort(array, low, pivot_index - 1);
		quick_sort(array, pivot_index + 1, high);
	}
}
```

## Makefile Rules

### Mandatory Rules
- `$(NAME)`: Build the main target
- `all`: Default rule, builds everything
- `clean`: Remove object files
- `fclean`: Remove all generated files
- `re`: Rebuild everything (fclean + all)

### Makefile Structure
```makefile
NAME = program_name

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCDIR = src
OBJDIR = obj
INCDIR = include

SOURCES = main.c utils.c parser.c
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
```

### Additional Rules
- **No unnecessary relinking**: Only rebuild when needed
- **Library compilation**: Auto-compile dependencies
- **Explicit source naming**: No wildcards like `*.c`
- **Multi-binary support**: Separate rules for multiple executables

## Validation Tools

### Norminette
The official norm checker for 42 projects:
```bash
# Install norminette
pip3 install norminette

# Check single file
norminette filename.c

# Check entire project
norminette .

# Check specific directory
norminette src/ include/
```

### Integration
- Available at: https://github.com/42School/norminette
- Python-based, open source
- Checks most (not all) norm rules
- Manual review required for subjective rules

## Common Mistakes

### Formatting Errors
```c
// ❌ Common mistakes
int function(int a,int b){    // Missing spaces, brace position
int*ptr;                      // Asterisk position
if(condition)                 // Missing space after keyword
	return(value);            // Missing space before parenthesis

// ✅ Correct formatting  
int	function(int a, int b)
{
	int	*ptr;
	
	if (condition)
		return (value);
}
```

### Structural Violations
```c
// ❌ Too many parameters
int complex_function(int a, int b, int c, int d, int e);

// ❌ Too many variables
int bad_function(void)
{
	int	a, b, c, d, e, f;  // Multiple issues: too many vars, multiple per line
}

// ❌ Function too long (>25 lines)
int massive_function(void)
{
	// ... 30+ lines of code
}
```

### Naming Violations
```c
// ❌ Wrong naming
int MyFunction(int CamelCase);          // Uppercase
typedef struct Node Node;               // Missing prefix
int tmp, i, x;                          // Non-descriptive

// ✅ Correct naming
int	my_function(int snake_case);
typedef struct s_node t_node;
int	temporary_value, index, coordinate;
```

## Best Practices

### Code Organization
1. **One logical task per function**
2. **Related functions in same file**
3. **Clear separation of concerns**
4. **Consistent error handling**

### Development Workflow
1. Write code following norm rules
2. Run norminette regularly
3. Test functionality
4. Peer review for subjective rules
5. Final norm compliance check

### Debugging Tips
- Use norminette early and often
- Check line endings and whitespace
- Verify indentation settings
- Count lines and parameters carefully
- Review variable alignment

---

## Quick Commands

```bash
# Check norm compliance
norminette .

# Compile with strict flags
cc -Wall -Wextra -Werror *.c

# Clean build
make fclean && make

# Check specific files
norminette src/*.c include/*.h
```

## Additional Resources

- **Official Repository**: https://github.com/42School/norminette
- **42 Intranet**: Project-specific norm requirements
- **Peer Learning**: Code reviews with classmates
- **Documentation**: C standard library references

---

**Remember**: The norm is not just about rules—it's about developing professional coding habits that will serve you throughout your programming career. Focus on understanding the "why" behind each rule to become a better developer.