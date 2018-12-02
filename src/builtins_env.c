/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_setenv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sblauens <sblauens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/24 02:23:18 by sblauens          #+#    #+#             */
/*   Updated: 2018/12/02 06:32:19 by sblauens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline void		printenv(const char **env)
{
	if (env)
		while (*env)
			ft_putendl(*env++);
}

/*
**  Builtin command to add or set a variable.
**
**  Returns 0 on succes and 1 on error.
*/

int						_builtin_setenv(const char **args, char ***env)
{
	if (args && !args[0])
		printenv((const char **)*env);
	else if ((args + 1) && args[1] && (args + 2) && args[2])
	{
		ft_putendl_fd("setenv: too many arguments\nUsage: VAR value", 2);
		return (1);
	}
	else if (_setenv(args, env) == -1)
		return (1);
	return (0);
}

/*
**  Builtin command to unset a variable.
**
**  Returns 0 on succes and 1 on error.
*/

int						_builtin_unsetenv(const char **args, char **env)
{
	if (!args || !*args)
	{
		ft_putendl_fd("unsetenv: too few arguments\nUsage: unsetenv VAR", 2);
		return (1);
	}
	if (_unsetenv(*args, env) == -1)
		return (1);
	return (0);
}

static inline int		opt0(const char **args, const char **env)
{
	if (args && *args)
	{
		ft_putendl_fd("env: cannot specify -0 with command", 2);
		return (1);
	}
	while (*env)
		ft_putstr(*env++);
	return (0);
}

/*
**  Builtin command to execute a path binary with a modified environment.
**
**  Print the environ if no arguments are passed.
**  '-0': output the environ without newlines.
**  '-i': execute with an empty environment.
**  Return 0 on succes and 1 on error.
*/

int						_builtin_env(const char **args, const char **env)
{
	char				**ep;

	if (!args || !args[0])
	{
		printenv(env);
		return (0);
	}
	else if (args && args[0] && **args == '-' && !(*args)[1])
		return (0);
	else if (args && args[0] && **args == '-' && (*args)[1] == '0')
		return (opt0(args + 1, env));
	else if (args && args[0] && **args == '-' && (*args)[1] == 'i')
	{
		ep = NULL;
		++args;
		if (!args || !*args)
			return (0);
	}
	else
		ep = cpyenv(env);
	bin_exec((char *const *)args, (char *const *)ep);
	if (ep)
		ft_strtabdel(&ep);
	return (0);
}