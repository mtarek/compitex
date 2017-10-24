                 ___                _ _____   __  __
                / __|___ _ __  _ __(_)_   _|__\ \/ /
               | (__/ _ \ '  \| '_ \ | | |/ -_)>  < 
                \___\___/_|_|_| .__/_| |_|\___/_/\_\

		Convert LaTeX equations to equivalent C-Code.

## Usage

```compitex filename```

```filename```  : A plain text file containing the LaTeX equations
            to be converted. Each equation on a separate line.

## Compiling

You need to have bison and flex installed to be able to
compile the code. After that just run make. 

## Acknowledgements 

This tool is based on Thomas Niemann's Lex & Yacc tutorial,
Many thanks to him for writing such a great document.

## Disclaimer

CompiTeX has not been actively maintained for a few years.
Currently, it is best used as a semi-auto solution in situations
where you would need to code a large number of equations.


