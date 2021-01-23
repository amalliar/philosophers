/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 10:56:17 by amalliar          #+#    #+#             */
/*   Updated: 2021/01/23 11:23:26 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <limits.h>

char		*ft_strchr(const char *str, int c)
{
	while (*str && *str != (char)c)
		++str;
	return ((*str == (char)c) ? (char *)str : NULL);
}

int			ft_atoi(const char *str)
{
	long long	num;
	int			negative;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		++str;
	if (*str == '-' || *str == '+')
	{
		negative = (*str == '-') ? 1 : 0;
		++str;
	}
	else
		negative = 0;
	num = 0;
	while (num <= INT_MAX && *str >= '0' && *str <= '9')
	{
		num = num * 10 + *str - '0';
		++str;
	}
	if (negative)
		return ((-num < INT_MIN || (*str >= '0' && *str <= '9')) ? 0 : -num);
	return ((num > INT_MAX || (*str >= '0' && *str <= '9')) ? -1 : num);
}
