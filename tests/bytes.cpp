
#include <iostream>
#include <cstdlib>

#include "Vriska/Core.hh"

static int	usage(std::string const & bin)
{
  std::cout << "Usage: " << bin << " num [toLittle]" << std::endl;
  return (1);
}

int main(int ac, char **av)
{
  bool	toBig = true;

  if (ac < 2)
    return (usage(av[0]));

  if (ac > 2)
    toBig = false;

  int	n = atoi(av[1]);
  std::cout << n << std::endl;

  std::string	n2 = Vriska::StringUtils::toBytes<unsigned int>(n, toBig);
  std::cout << Vriska::StringUtils::escape(n2) << std::endl;

  std::cout << Vriska::StringUtils::fromBytes<unsigned int>(n2, toBig) << std::endl;
  return (0);
}
