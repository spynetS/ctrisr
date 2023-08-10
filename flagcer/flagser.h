
/*
    Alfred Roos 2023-02-16
    TO USE 

    to add a flag run 
    addFlag("-ex","--example", "this is a example", exampleFunction);
    the exampleFunction is a function you create wich will be run when the parser finds the flag
    define a function like this

    void exampleFunction(int argc, char** argv){}

    the arguments is a number of how many args the flag got
    and the argv is a string array if the values

    to add a default help flag run
    addHelp();
    this till just add a template help flag which will print all the defined flags

 */


void addFlag(char* name, char* longname, char* description, void *oncall);
void addHelp();
int  parse(int argc, char** argv);
void printHelp(int argc, char** args);


