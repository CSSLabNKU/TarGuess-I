# TarGuess-I
Code for our 2016 ACM CCS Paper "Targeted Online Password Guessing: An Underestimated Threat"

We do not upload the source files including personal identifiable information, treating each individual account as confidential, storing and processing them on computers not linked to the Internet.

While these datasets might be already exploited by attackers for misconduct, our use is helpful for security administrators/users to measure password strength and prevent weak ones.

### How to Run the Code 

#### Train Process 

You can choose to run the `./TarGuess_I_Train/targuess1_train.exe` file directly after configuring the `cogfig.ini`.

- `training_set_path`: the path to the training set file, which is in CSV format.
  - **Note:** Each line follows this format: `email\tpassword\tname\tID card\tusername\tphone number\tbirthday`, with tab (\t) as the delimiter between each field, such as `fakename@gmail.com fakepassword	fake|name|	150304196407116999	fname	13863749572	19640711` is a typical example with fake information. If a field is missing, it should be replaced with an empty string. 
  - The training set file should be collected and cleaned by yourself. For ethical reasons, we have not provided the corresponding file.
- `model_output_path`: the output path for the trained model.
- `print_info_interval`: the interval for printing output information

Then, you can run the `exe` file in the Windows terminal.

```powershell
./targuess1_train.exe 
```
**Alternatively**, if you want to modify the source code and recompile, you need to navigate to the `src` directory, make the necessary changes, recompile, and then execute the output. Here is the compilation process in Windows.

```powershell
cd ./src
g++ ./targuess1_train.cpp ./timer.cpp ./person.cpp -o ../targuess1_train.exe
```

#### Guess Process 

You can choose to run the `./TarGuess_I_Guess/targuess1_guess.exe` file directly after configuring the `cogfig.ini`. The key properties of the configuration file are as follows:

- `model_path`: the path where the model training output is saved. We have copied the output files to the current `TarGuess_I_Guess` folder.
- `personal_info_path`: the path to the personal information `csv` file, and its format is the same as the one used for the training set described above.
- `output_to_file`: the path to the output file with guessing result

Then, you can run the `exe` file in the Windows terminal.

```powershell
./targuess1_guess.exe 
```

**Alternatively**, if you want to modify the source code and recompile, you need to navigate to the `src` directory, make the necessary changes, recompile, and then execute the output. Here is the compilation process in Windows.

```powershell
cd ./src
g++ ./targuess1_guess.cpp ./person.cpp ./personTran.cpp ./timer.cpp -o ../targuess1_guess.exe
```

### Research Paper

The paper is available at the [ACM Digital Library](https://dl.acm.org/doi/10.1145/2976749.2978339).

<b>If you use any part of our codes, you are committed to cite the following paper:</b>

```latex
@inproceedings{wang2016targuess,
    author      = {
        Wang, Ding and 
        Zhang, Zijian and 
        Wang, Ping and 
        Yan, Jeff and
        Huang, Xinyi
    },
    booktitle   = {ACM SIGSAC Conference on Computer and Communications Security},
    title       = {{Targeted Online Password Guessing: An Underestimated Threat}},
    year        = {2016},
    pages       = {1242–1254},
    doi         = {10.1145/2976749.2978339}
}
