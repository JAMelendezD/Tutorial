# Libraries

import MDAnalysis as mda
import argparse

# Functions

def com():
    print('Not implemented')

def r_gyr():
    print('Not implemented')

# Entry point

def main():

    # Arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('--top', '-s', required = True, type=str, help='tpr file')
    parser.add_argument('--traj', '-f', required = True, type=str, help='trajectory file')
    args = parser.parse_args()

    # Load trajectory 
    print(f'MDA version: {mda.__version__}')
    u = mda.Universe(args.top, args.traj)
    len_traj = len(u.trajectory)
    print('Number of frames in the trajectory: ', len_traj)
    
    # Create selection
    sel = u.select_atoms('protein')
    print('Number of atoms in the selection: ', len(sel))

    # Iterate over all the frames and calculate the radius of gyration
    for ts in u.trajectory:
        # Do something with the data
        print(ts.time)

if __name__ == '__main__':
    main()
