/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptionErrors.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:52:15 by mamazzal          #+#    #+#             */
/*   Updated: 2023/12/27 13:52:35 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONERRORS_HPP
#define EXCEPTIONERRORS_HPP

#include "main.h"


class HttpConfig : std::exception {
  public : 
    const char *what() const throw() {
      return "Exception Error : bad http config";
  }
};

class confFileError : std::exception {
  public : 
    const char *what() const throw() {
      return "Exception Error : unvalid config file";
  }
};

#endif
