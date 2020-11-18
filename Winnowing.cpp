#include "Winnowing.h"

static int hashK_gram(const std::string& instr, std::vector<std::size_t>& hashvec){
    std::hash<std::string> hash_string;
    if(!hashvec.empty())
        hashvec.clear();
    for(std::size_t i = 0; i < instr.length() - K_s + 1; ++i){
        hashvec.emplace_back(hash_string(instr.substr(i, K_s)));
    }
    return EXIT_OK;
}

static int winnow(const std::vector<std::size_t>& hashvec, std::vector<std::pair<std::size_t, std::size_t>>& fpvec){
    std::size_t hashindex = 0;
    std::vector<std::size_t> window(W_s, SIZE_MAX);
    auto next_hash = [&hashindex,&hashvec](){
        if(hashindex < hashvec.size()){
            return hashvec[hashindex++];
        }
        return SIZE_MAX;
    };
    int r = 0;
    int mini = 0;
    while(true){
        r = (r + 1) % W_s;
        if((window[r] = next_hash()) == SIZE_MAX)
            break;
        if(mini == r){
            std::size_t htmpindex = hashindex - 1;
            std::size_t hfinalindex = hashindex - 1;
            for(int i = (r - 1) % W_s; i != r; i = (i - 1 + W_s) % W_s){
                htmpindex--;
                if(window[i] < window[mini]){
                    mini = i;
                    hfinalindex = htmpindex;
                }
            }
            fpvec.emplace_back(std::make_pair(window[mini], hfinalindex));
        }
        else{
            if( window[r] < window[mini] ){
                mini = r;
                fpvec.emplace_back(std::make_pair(window[mini], hashindex - 1));
            }
        }
    }
    return EXIT_OK;
}

int winnowing(const std::string& instr, std::vector<std::pair<std::size_t, std::size_t>>& fpvec){
    //get k-gram hashs
    std::vector<std::size_t> hashvec;
    int hashstatus = hashK_gram(instr, hashvec);
    if(hashstatus != EXIT_OK){
        std::cerr << "codesim: hash string: " << instr << " failed." << std::endl;
        return hashstatus;
    }
    if(hashvec.empty()){
        std::cerr << "codesim: hashs are null because instr is too short."<< std::endl;
        return EXIT_FAILURE;
    }

    //winnow
    int winstatus = winnow(hashvec, fpvec);
    if(winstatus != EXIT_OK){
        std::cerr << "codesim: calculate fingerprints of " << instr << " failed." << std::endl;
        return winstatus;
    }

    return EXIT_OK;
}
