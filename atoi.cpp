/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 19:12:33 by mamazzal          #+#    #+#             */
/*   Updated: 2024/01/18 18:08:20 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int _atoi_(std::string  str)
{
  int ismis = 1;
  size_t count = 0;
  int result = 0;
  if (str[0] == '-' || str[0] == '+') {
    if (str[0] == '-')
      ismis = -1;
    count++;
  }
  while (std::isdigit(str[count]) && count < str.length()) {
    result = (result * 10) + str[count] - 48;
    count++;
  }

  result = result * ismis;

  return result;
}

//intger to string
std::string _itos_(int n)
{
  std::string str;
  int i = 0;
  int ismis = 1;
  if (n < 0) {
    ismis = -1;
    n = n * ismis;
  }
  while (n > 0) {
    str[i] = (n % 10) + 48;
    n = n / 10;
    i++;
  }
  if (ismis == -1)
    str[i] = '-';
  return str;
}
