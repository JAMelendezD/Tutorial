# ------------------------------------------------------------------------------
#                 Julian Melendez make_ndx.py (2022)
#          make_index.py : Creates an index file from tinker xyz
# ------------------------------------------------------------------------------

import numpy as np
import readline
import os

logic_info = {"&"      : "Logical and operator (separated by white space)", 
              "|"      : "Logical or operator (separated by white space)",
              "-"      : "Subtracts two groups (separated by white space)",
              ":"      : "Assign operator (no white space)"}

sele_info =  {"a"      : "Atom name",
              "t"      : "Atom type",
              "g"      : "Index group"}

other_info = {"n"      : "Rename selected group",
              "d"      : "Delete selected group",
              "q"      : "Quit and save the index",
              "Enter"  : "List groups"}

I = "\x1b[38;2;255;200;105;208m"    # Information color (Yellow)
G = "\x1b[38;2;120;255;105;208m"    # Selection color (Green)
O = "\x1b[38;2;120;105;235;208m"    # Not used (Blue) 
E = "\x1b[38;2;255;55;50;208m"      # Error color (Red)
R = "\x1b[0m"                       # Reset color

def backup(fname : str, counter : int) -> str:
    '''
    Uses recursion to check if file name exists. If it exists then it
    checks if the backup also exists and so on. Returns a file name
    to create a backup with a name that does not exist 
    '''
    exists = os.path.exists('{}'.format(fname))
    if exists ==  False:
        return fname
    else:
        fname = fname.split('#')[-1]
        fname = fname.split('_')[0]
        fname = '#' + fname + '_' + str(counter)
        counter += 1
        return backup(fname, counter)

def read_index(index : str) -> dict:
    '''
    Reads index file and converts it to a dicionary were the key
    is the name of the group and the value is a list of indeces
    '''
    dic = {}
    with open(index, 'r') as f:
        for line in f:
            if line.startswith("["):
                key = line[line.find('[')+1:line.find(']')].strip()
                dic[key] = []
            else:
                dic[key] += line.split()   
    return dic

def write_index(index : dict, out : str) -> None:
    '''
    Uses backup to get an appropriate name for the output. Then it takes the
    index and prints it to the output file
    '''
    new_name = backup(out, 0)
    if out == new_name:
        os.system(f"touch {out}")
    else:
        print(I+f"Found a file with the same name backing it up to {new_name}"+R)
        os.system(f"mv {out} '{new_name}'")
        os.system(f"touch {out}")

    f = open(out, 'w')

    for key in index.keys():
        f.write(f"[ {key:^s} ]")
        for i, ind in enumerate(index[key]):
            ind = int(ind) + 1
            if  i  % 15 == 0:
                f.write(f"\n{ind:>4d}")
            else:
                f.write(f" {ind:>4d}")
        f.write("\n")
    return

def read_xyz(xyz : str) -> tuple[np.array(int), np.array(int)]:
    '''
    Reads input xyz and extracts the atom names and the atoms types
    '''
    # Gets the number of atoms
    with open(xyz, 'r') as f:
        for count, line in enumerate(f):
            if count == 0:
                num_atoms = int(line.split()[0])
                break

    # Count the number of lines
    with open(xyz, 'r') as f:
        for count, line in enumerate(f):
            pass
    count += 1

    # Checks if the box has 1 or 2 header lines
    print(80*"#")
    if num_atoms + 1 == count:
        print(I+"XYZ without box information"+R)
        ignore = 1
    elif num_atoms + 2 == count:
        print(I+"XYZ with box information"+R)
        ignore = 2
    else:
        print(E+"XYZ seems to be incorrected or not in tinker format"+R)
        exit(1) 

    print(I+f"Number of atoms: {num_atoms:10d}"+R)

    # If files follows tinker format names and types
    # can be extracted as follows
    try:
        names = np.loadtxt(xyz, usecols = [1], dtype = str, skiprows = ignore)
        types = np.loadtxt(xyz, usecols = [5], dtype = int, skiprows = ignore)
    except:
        print(E+"XYZ seems to be incorrected or not in tinker format"+R)
        exit(1) 

    return names, types

def get_index_values(command : str, selection : dict, index : dict) -> list:
    '''
    Based on the available options it computes the appropriate index list.
    if a: matches the name to the list of names or a letter to start with 
          using the *
    if t: matches the atom type to the list of types
    if g: based on the current index returns the matching list
    '''
    if ":" not in command:
        print(E+"Delimiter separating selection and value not found"+R)
        return
    else:
        option = command.split(":")[0]
        value = command.split(":")[1]
    
    if option == 'a':
        tmp = value.split("*")
        if len(tmp) == 1:
            return list(np.array(np.where(names == value), dtype = str)[0])
        else:
            bool_arr = np.char.startswith(names, tmp[0])
            return list(np.array(np.where(bool_arr), dtype = str)[0])
    elif option == 't':
        try:
            value = int(value)
            return list(np.array(np.where(types == value), dtype = str)[0])
        except:
            print(E+"The type given cant be converted into an integer"+R)
            return 
    elif option == 'g':
        num = int(value)
        if num in selection.keys():
            return index[selection[num]]
        else:
            print(E+f"Selection group {value:s} not found"+R)
            return
    else:
        print(E+f"Selection option {option:s} not available"+R)
        return 

def evaluate_pair(group1 : list, group2: list, operator) -> list:
    '''
    Takes two lists and one operator. Converts the list to sets and
    applies the operation between them. Returns the result as a sorted list
    '''
    if operator not in logic_info.keys():
        return
    expression = f"set(group1) {operator} set(group2)"
    result = list(eval(expression))
    result.sort(key = int)
    return result

def evaluate_expression(exp : str, selection: dict, index : dict) -> tuple[list, str]:
    '''
    Uses evaluate pair to evaluate any expression by splitting into expressions
    of two lists and one operator. 
    '''
    elements = exp.split()
    name = ""
    if len(elements) % 2 == 0:  # Expressions should be uneven number of terms
        print(E+"Malformed expression"+R)
        return
    else:
        # This two variables are needed to get the first case
        # since the empty set or any other set gives the other
        # set therefore for the first case the result is just the input set
        # after that it then evaluates the remaining operators
        group1 = []
        operator = '|'
        for i in range(0, len(elements), 2):
            group2 = get_index_values(elements[i], selection, index)
            if group2 == None:
                return None, None
            group1 = evaluate_pair(group1, group2, operator)
            name += elements[i]
            if i < len(elements) - 1:
                operator = elements[i + 1]
                name += "_" + operator + "_"
                if operator not in logic_info.keys():
                    print(E+f"Operator ({operator:s}) not available"+R)
                    return None, None
    
    return group1, name

def update_selection(index : dict) -> dict:
    '''
    Gets called everytime index changes. It updates the selection dictionary
    based on the modified index
    '''
    selection = {}
    for i, key in enumerate(index.keys()):
        selection[i] = key 
    return selection

def print_options(index : dict) -> None:
    '''
    Outputs to stdout all the options of the program as well as the index
    groups
    '''
    print(80*"#")
    print(G+"Available selection groups:\n"+R)
    for i, key in enumerate(index.keys()):
        print(G+f"{i:3d}: {key:50s} ({len(index[key]):8d} Atoms)"+R)
    print("\n"+80*"#")
    print("Available logical operators:")
    for key, value in logic_info.items():
        print(f"({key:^5s}) {value:s}")
    print(80*"#")
    print("Available selection options:")
    for key, value in sele_info.items():
        print(f"({key:^5s}) {value:s}")
    print(80*"#")
    print("Other options:")
    for key, value in other_info.items():
        print(f"({key:^5s}) {value:s}")
    print(80*"#")

def print_examples():
    print(80*"#")
    print(O+"Examples:"+R)
    print(O+"g:0 | g:1     (group 0 or group 1)"+R)
    print(O+"a:N*          (all names that start with N)"+R)
    print(O+"a:O* & t:54   (start with O and type 54)"+R)
    print(O+"a:OW | a:HW   (selects all water)"+R)
    print(O+"g:0 - g:1     (all - water gives non water atoms)"+R)

if __name__ == "__main__":

    from argparse import ArgumentParser

    parser = ArgumentParser(description="Creates and index file based on a tinker xyz.")
    parser.add_argument("--xyz","-f",help="tinker XYZ file",type=str,required=True)
    parser.add_argument("--ndx","-n",help="Index file",type=str,default=None)    
    parser.add_argument("--out","-o",help="Output file",type=str,default="index.ndx")
    args = parser.parse_args()

    names, types = read_xyz(args.xyz)
    if args.ndx != None:
        index = read_index(args.ndx)
    else:
        index = {}

    print_examples()
    
    all = list(np.array(np.arange(0, len(names)), dtype = str))
    index["all"] = all
    selection = update_selection(index)
    print_options(index)

    while True:
        inp = input("")
        if inp.strip() == "q":
            write_index(index, args.out)
            break
        elif inp.strip() == "":
            print_options(index) 
        elif inp.strip()[0:2] in ["n:", "d:"]:
            if inp.strip()[0:2] == "d:":
                try:
                    value = int(inp.split(":")[-1])
                except:
                    print(E+"Value given to delete cant be converted to an integer"+R)
                if value in selection.keys():
                    index.pop(selection[value])
                    selection = update_selection(index)
                else:
                    print(E+"Group to delete was not found"+R)
            else:
                tmp = inp.split()
                name = tmp[-1].strip()
                if len(tmp) == 1:
                    print(E+"No name given. Cant rename group"+R)
                else:
                    try:
                        value = int(tmp[0].split(":")[-1])
                    except:
                        print(E+"Value given to delete cant be converted to an integer"+R)
                    if value in selection.keys():
                        old_keys = list(index.keys())
                        num = list(selection.keys())
                        for i, key in zip(num, old_keys):
                            if i == value:
                                index[name] = index.pop(selection[i])
                            else:
                                index[selection[i]] = index.pop(selection[i])
                        selection = update_selection(index)
                    else:
                        print(E+"Group to rename was not found"+R)
        else:
            new_group, name = evaluate_expression(inp, selection, index)
            if new_group != None:
                if len(new_group) == 0:
                    print(E+f"Empty group {name:s} not created"+R)
                else:
                    if name not in index.keys():
                        index[name] = new_group # Adds new group to index
                        selection = update_selection(index)
                    else:
                        print(E+f"Repeated group {name:s} not created"+R)