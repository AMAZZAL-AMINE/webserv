/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:44:16 by mamazzal          #+#    #+#             */
/*   Updated: 2023/12/27 10:55:35 by mamazzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int main(int argc, char __unused **argv) {
  if (argc != 2) {
    std::cerr << "Error : config file\n";
    return 1;
  }
  try {
  } catch (std::exception & e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}