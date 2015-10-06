/*********************************************************************************
* Copyright (C) 2015 Alexey V. Akimov
*
* This file is distributed under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of
* the License, or (at your option) any later version.
* See the file LICENSE in the root directory of this distribution
* or <http://www.gnu.org/licenses/>.
*
*********************************************************************************/

#ifndef CONTROL_PARAMETERS_H
#define CONTROL_PARAMETERS_H

#include "../../../../mmath/libmmath.h"
using namespace libmmath;


namespace libhamiltonian{
namespace libhamiltonian_atomistic{
namespace libhamiltonian_qm{
namespace libcontrol_parameters{



class excitation{

  public:
  int size;                // 1 = single, 2 = double, etc
  vector<int> from_orbit;  // indices of the orbitals from which excitation is performed: -1 = HOMO-1, 0 = HOMO, 1 = LUMO, etc.
  vector<int> from_spin;   // same as above, but for spin: 1 = spin up, -1 = spin down
  vector<int> to_orbit;    // indices of the orbitals to which excitation is performed: -1 = HOMO-1, 0 = HOMO, 1 = LUMO, etc.
  vector<int> to_spin;     // same as above, but for spin: 1 = spin up, -1 = spin down

  double Energy;           // electronic energy of this configuration
  double f;                // oscillator strength

  // Example of exitation:
  // for convenience, in the input spin components is given by a letter: A = 1, B = -1
  // { {0, 0}, {1, -1}, {1, 1}, {1, -1} } = double excitations from HOMO to LUMO with conservation of spin multiplicity

  // dafault constructor
  excitation() { size = 0; Energy = 0.0; f = 0.0; }
  
  // single excitations
  excitation(int _f_o, int _f_s, int _t_o, int _t_s){ 
    size = 1;
    Energy = 0.0;
    f = 0.0;
    from_orbit = vector<int>(1,_f_o);
    from_spin = vector<int>(1,_f_s);
    to_orbit = vector<int>(1,_t_o);
    to_spin = vector<int>(1,_t_s);
    cout<<"Constructing single excitation: "<<_f_o<<" "<<_f_s<<" -> "<<_t_o<<" "<<_t_s<<endl;
  }



};





class Control_Parameters{

public:
//---------- Members --------

  //----------------- All simulation parameters and flags (set to default values) -------------------
  // <calculations>
  std::string runtype;           // Possible: scan, dos, md, opt, nac, test1
  std::string hamiltonian;       // Possible: eht, indo, cndo2, etc.
  std::string spin_method;       // Possible:  restricted, unrestricted
  int DF;                        // Debug flag: if set to 1 - will print a lot of information. Be carefull!!!
  // </calculations>

  // <guess>
  std::string guess_type;        // Define how to make guess orbitals
                                 // Possible: sad
  // </guess>

  // <scf_options>
  std::string scf_algo;          ///< algorithm for SCF iterations
  int use_disk;                  ///< write temporary variables to disk instead of RAM - this can help reducing memory costs
  int use_rosh;                  ///< use restricted open-shell
  int do_annihilate;             ///< do spin annihilation at the last iteration
  int pop_opt;                   ///< option for how to populate energy levels: 0 - integer, 1 - Fermi
  int use_diis;                  ///< flag to turn on/off diis
  int diis_max;                  ///< dimension of DIIS matrix, if used
  int diis_start_iter;           ///< iteration after which diis will start
  int use_level_shift;           ///< flag to turn on/off level shifting
  double shift_magnitude;        ///< magnitude of the energy level shifts, if used
  int use_damping;               ///< flag to turn on/off damping        
  int damping_start;             ///< number of (standard) iterations before damping is in effect
  double damping_const;          // parameter for damping, if used
  double etol;                   // energy convergence criterium, [Ha] 
  double den_tol;                // density convergence criterium [?]
  int Niter;                     // maximal number of SCF iterations
  double degen_tol;              // amount of population difference between two levels, when one can say the two levels are degenerate
  // </scf_options>

  // <hamiltonian_options>
  std::string parameters;        // Name of the file that contains parameters for Hamiltonian
  std::string eht_params_format; // Format of the file that contains parameters for EHT Hamiltonian (see more detailed description in the manual)
                                 // "eht+0"   :  minimal EHT format (default)
                                 // "eht+n"   :  add n more parameters at the end of each line, the meaning of the parameters
                                 //              may be different, depending on which method is used
                                 // "eht+n+K" :  same as eht+n, but also use K_ij as parameters - so far this is most general


  int eht_formula;               // formula for EHT Hamiltonian:
                                 // 0 - unweighted 
                                 // 1 - weighted
                                 // 2 - Calzaferi correction
                                 // 3 - my method (testing!)

  int eht_sce_formula;           ///< how to treat self-consistent electrostatics for EHT,
                                 ///< 0 - no self-consistent electrostatics
                                 ///< 1 - total-charge dependent IP
                                 ///< 2 - orbital-reolved corrections
                                 ///< 3 - my addition of perametric exchange 

  int eht_fock_opt;              ///< how to treat EHT Hamiltonian (H_eht)
                                 ///< 0 - as a Fock matrix (F_eht = H_eht) - no correction to self-consistency is needed, but the actual energy functional is different
                                 ///< 1 (default) - as a model Hamiltonian (F_eht = 2*H_eht - H_eht0) - correction for self-consistency is needed

  int eht_electrostatics;        ///< how to describe additional electrostatic interactions
                                 ///< 0 - no additional field effect
                                 ///< 1 - include pairwise Coulombic effects via Mulliken charges
  // </hamiltonian_options>

  // <properties>
  int compute_vertical_ip;       
  int compute_vertical_ea; 
  // </properties>

  // <md_options>
  double md_dt;                  // integration time step for MD [input in fs, internally in a.u.]
  int md_nsteps;                 // number of time steps for MD
  // </md_options>

  // <opt_options>
  double opt_dt;                 // integration time step for optimization [input in fs, internally in a.u.]
  int opt_nsteps;                // number of time steps for optimization
  // </opt_options>

  // <multipole_options>
  int compute_dipole;            // flag to turn dipole moment calculations
  // </multipole_options>

  // <dos_options>
  int compute_dos;               // flag to turn DOS calculations on/off
  std::string dos_opt;           // option for DOS comutation: "dens" - based on density matrix, "wfc" - based on wavefunction
  std::string dos_prefix;        // Prefix for the files in which atomic-projected DOS will be written  
  // </dos_options>

  // <charge_density_options>
  int compute_charge_density;         // flag to turn computation of charge density on
  int nx_grid, ny_grid, nz_grid;      // Number of voxels along each direction
  std::string charge_density_prefix;  // Prefix for the files in which CUBE orbitals will be written  
  vector<int> orbs;
  // </charge_density_options>


  // <nac_options>
  std::string nac_md_trajectory_filename;  // Name of the file that contains coordinates of the MD trajectory (in xyz format)
  std::string nac_prefix;        // Prefix of all files into wich NACs for different time steps will be written
  int nac_min_frame;             // index of minimal MD snapshot to include in NAC
  int nac_max_frame;             // index of maximal MD snapshot to include in NAC
  vector<int> nac_min_orbs;      // indexes of minimal orbitals to include in NAC matrix - for each fragment
  vector<int> nac_max_orbs;      // indexes of maximal orbitals to include in NAC matrix - for each fragment
  double nac_dt;                 // time step separating MD point in precomputed trajectory [fs]
  int nac_opt;                   // how to compute NACs between non-orthonormal orbitals
  // </nac_options>

  // <scan_options>
  int scan_mov_at;               // index of the atom that will be moved
  int scan_ref_at;               // index of the atom that serves as the reference atom
  VECTOR scan_dir;               // direction of the scan
  double scan_dxmin;             // initial displacement along the scan direction w.r.t position of the reference atoms
  double scan_dxmax;             // final displacement along the scan direction w.r.t position of the reference atoms
  double scan_dx;                // increment of displacement
  // </scan_options>

  // <excitations>
  int compute_excitations;       // flag turning on/off the actual computation of excitations
  int num_excitations;           // number of excitations to consider
  std::string excitations_opt;   // option for how to compute excitation energies
  double spectral_width;         // parameter for spectra calculation - width of the smearing
  vector<excitation> excitations; 
  // </excitations>

  // <unit_cell>
  VECTOR t1,t2,t3;               // vectors of periodic translations in 3 directions
  int x_period;                  // if periodic along t1
  int y_period;                  // if periodic along t2
  int z_period;                  // if periodic along t3
  // </unit_cell>

  // <coordinates>
  int Natoms;                    // Number of atoms
  double charge;                 // total charge of the system (-number of excess electrons)
  int spin;                      // 1 = singlet, 2 = doublet, 3 = triplet, etc.
  std::string coordinates;       // Direct or Cartesian
                                 // actual coordinates are stored separately
  // </coordinates>

  // <fragments>
  vector< vector<int> > fragments;  // list of atomic indices for atoms that are included in given fragment
  vector< int >         frag_size;  // size of the fragments
  vector< std::string>  frag_name;  // names of the fragments
  vector< double >      frag_charge;// charges of the fragments, sum must be equal to charge - see <coordinates> sections
  // </fragments>
  


//--------- Methods ----------
    Control_Parameters();
   ~Control_Parameters(){ ;; }
    Control_Parameters(const Control_Parameters& x){ *this = x; }

};


void get_parameters_from_file(std::string, Control_Parameters&);


}// namespace libhcontrol_parameters
}// namespace libhamiltonian_qm
}// namespace libhamiltonian_atomistic
}// namespace libhamiltonian






#endif // CONTROL_PARAMETERS_H
