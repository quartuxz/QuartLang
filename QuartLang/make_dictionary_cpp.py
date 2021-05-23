



#generates a hard coded c++ map with all the lambda tokens
if __name__ == "__main__":
    filename = input("Enter the file to translate:")
    nameWithNoExtension = filename.split(".")
    outputFile = open(nameWithNoExtension[0]+".cpp","w")
    
    inputFile = open(filename, "r")
    lines = inputFile.readlines()
    
    
    outStr = "#include <map> \n std::map<std::string,Token> "+nameWithNoExtension[0]+" = { "
    
    for x in lines:
        
        final = x.replace("\n","")
        outStr += '{"'+final+'", Token::lambdaTok'+"},\n"
    
    
    outStr += "};"

    outputFile.write(outStr)


