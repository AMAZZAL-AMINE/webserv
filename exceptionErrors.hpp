/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptionErrors.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:52:15 by mamazzal          #+#    #+#             */
/*   Updated: 2024/02/22 16:45:37 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONERRORS_HPP
#define EXCEPTIONERRORS_HPP

#include "main.h"


class HttpConfig : public std::exception {
  public : 
    const char *what() const throw() {
      return "Exception Error : bad http config";
  }
};

class confFileError : public std::exception {
  public : 
    const char *what() const throw() {
      return "Exception Error : config file";
  }
};

class MethodsException : public std::exception {
  public : 
    const char *what() const throw() {
      return "ConfigFile : methods";
  }
};



#endif
