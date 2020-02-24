#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match( program->type, reNum ) ){
        std::cout<<"const "<<destReg<<" "<<program->type<<std::endl;
        
    }else if( regex_match( program->type, reId ) ){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<zero<<std::endl;

    }else if(program->type=="Param"){
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;

    }else if(program->type=="Seq"){
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }
    
    // TODO : handle the others
    }else if(program->type=="If"){
        std::string label_1 = makeName("label");
        std::string label_2 = makeName("label");
        std::string val_1 = makeName("val");
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        // C
        CompileRec(val_1, program->branches[0]);
        // 
        std::cout<<"beq "<<val_1<<" "<<zero<<" "<<label_1<<std::endl;
        // X
        CompileRec(destReg, program->branches[1]);
        // jmp label2
        std::cout<<"beq "<<zero<<" "<<zero<<" "<<label_2<<std::endl;
        // :label1
        std::cout<<":"<<label_1<<std::endl;
        // Y
        CompileRec(destReg, program->branches[2]);
        // :label2
        std::cout<<":"<<label_2<<std::endl;

    }else if(program->type=="Input"){
        std::cout << "input " << destReg << std::endl;

    }else if(program->type=="Output"){
        CompileRec(destReg, program->branches[0]);
        std::cout << "output " << destReg << std::endl;

    }else if(program->type=="LessThan"){
        std::string val_1 = makeName("val");
        std::string val_2 = makeName("val");
        CompileRec(val_1, program->branches[0]);
        CompileRec(val_2, program->branches[1]);
        std::cout << "lt " << destReg << " " << val_1 << " " << val_2 << std::endl;

    }else if(program->type=="Add"){
        std::string val_1 = makeName("val");
        std::string val_2 = makeName("val");
        CompileRec(val_1, program->branches[0]);
        CompileRec(val_2, program->branches[1]);
        std::cout<<"add "<<destReg<<" "<<val_1<<" "<<val_2<<std::endl;

    }else if(program->type=="Sub"){
        std::string val_1 = makeName("val");
        std::string val_2 = makeName("val");
        CompileRec(val_1, program->branches[0]);
        CompileRec(val_2, program->branches[1]);
        std::cout<<"sub "<<destReg<<" "<<val_1<<" "<<val_2<<std::endl;

    }else if(program->type=="Assign"){
        CompileRec(program->value, program->branches[0]);
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->value<<" "<<zero<<std::endl;

    }else if(program->type=="While"){
        
        std::string val_1 = makeName("val");
        std::string val_2 = makeName("val");
        std::string label_1 = makeName("label");
        std::string label_2 = makeName("label");
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        // :label_1
        std::cout<<":"<<label_1<<std::endl;
        // C
        CompileRec(val_1, program->branches[0]);
        // 
        std::cout<<"beq "<<val_1<<" "<<zero<<" "<<label_2<<std::endl;
        // X
        CompileRec(val_2, program->branches[1]);
        // jmp label_1
        std::cout<<"beq "<<zero<<" "<<zero<<" "<<label_1<<std::endl;
        // :label2
        std::cout<<":"<<label_2<<std::endl;
        std::cout<<"const "<<destReg<<" 0"<<std::endl;

    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");
    
    // Compile the whole thing
    CompileRec(res, program);
    
    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}
