#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
        
    // TODO : Check for things matching reId
    }else if(regex_match(program->type, reId)){
        return context.bindings[program->type.c_str()];

    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
        
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
    // TODO: Handle other constructs
    }else if(program->type=="LessThan"){
        int32_t val_1 = Interpret(context, program->branches.at(0));
        int32_t val_2 = Interpret(context, program->branches.at(1));
        return val_1 < val_2 ? 1 : 0;

    }else if(program->type=="Seq"){
        unsigned last_index = program->branches.size() - 1;
        for(unsigned i=0; i<last_index; i++) {
            Interpret(context, program->branches.at(i));
        }
        return Interpret(context, program->branches.at(last_index));

    }else if(program->type=="Assign"){
        int32_t val = Interpret(context, program->branches.at(0));
        context.bindings[program->value] = val;
        return val;
    }else if(program->type=="Input"){
        int32_t val;
        std::cin >> val;
        return val;

    }else if(program->type=="Add"){
        int32_t val_1 = Interpret(context, program->branches.at(0));
        int32_t val_2 = Interpret(context, program->branches.at(1));
        return val_1 + val_2;
    
    }else if(program->type=="Sub"){
        int32_t val_1 = Interpret(context, program->branches.at(0));
        int32_t val_2 = Interpret(context, program->branches.at(1));
        return val_1 - val_2;
    
    }else if(program->type=="If"){
        int32_t val_1 = Interpret(context, program->branches.at(0));
        int32_t val_2;
        if (val_1){
            val_2 = Interpret(context, program->branches.at(1));
        } else {
            val_2 = Interpret(context, program->branches.at(2));
        }
        return val_2;
     }else if(program->type=="While"){
        while (Interpret(context, program->branches.at(0))) {
            Interpret(context, program->branches.at(1));
        }
        return 0;

    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
