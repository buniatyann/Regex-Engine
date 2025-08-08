#ifndef MATCH_HPP
#define MATCH_HPP

#include <cstddef>

namespace match {

using size_t = std::size_t;

class match {
public:
    match(bool matched, size_t start, size_t end);
    bool is_matched() const { 
        return matched_; 
    }
    
    size_t get_start() const { 
        return start_; 
    }
    
    size_t get_end() const { 
        return end_; 
    }

private:
    bool matched_;
    size_t start_;
    size_t end_;
};

} // namespace match

#endif // MATCH_HPP