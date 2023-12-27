/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 19:12:33 by mamazzal          #+#    #+#             */
/*   Updated: 2023/12/27 19:26:55 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int _atoi_(std::string  str) {
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