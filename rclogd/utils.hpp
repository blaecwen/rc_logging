#ifndef UTILS_HPP
#define UTILS_HPP

namespace rclog {

#ifdef DEBUG
#define DOUT std::cerr
#else
#define DOUT if(true){} else std::cerr
#endif


}

#endif // UTILS_HPP
