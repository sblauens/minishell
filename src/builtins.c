/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sblauens <sblauens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 01:49:51 by sblauens          #+#    #+#             */
/*   Updated: 2019/02/15 17:06:56 by sblauens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "libft.h"
#include "minishell.h"

/*
**  Change the current directory
**
**  The environment variables 'OLDPWD' and 'PWD'
**  will be set accordingly.
**  Return 0 on succes and a value <= -1 on error.
*/

static inline int		chpwd(const char *path, char ***env)
{
	int					r;
	char				*buf;

	buf = getcwd(NULL, 0);
	if (!(r = chdir(path)))
	{
		r += envset((const char *[2]){"OLDPWD", buf}, env);
		free(buf);
		buf = getcwd(NULL, 0);
		r += envset((const char *[2]){"PWD", buf}, env);
	}
	free(buf);
	return (r);
}

/*
**  Builtin to change the current working directory.
**
**  Synopsis: cd [-] | [DIRECTORY]
**
**  '-': change to $OLDPWD.
**  Return 0 on succes and 1 on error.
*/

int						builtin_cd(const char *path, char ***env)
{
	struct stat			sb;

	if (!path && (!(path = envget("HOME", (const char **)*env)) || !*path))
		return (0);
	else if (*path == '-' && !*(path + 1)
			&& (!(path = envget("OLDPWD", (const char **)*env)) || !*path))
		path = ".";
	if (!chpwd(path, env))
		return (0);
	if (stat(path, &sb) == -1)
		puterr("cd: ",
				"cannot change directory: missing or permission denied: ",
				path);
	else if ((sb.st_mode & S_IFMT) != S_IFDIR)
		puterr("cd: ", "not a directory: ", path);
	else if (!(sb.st_mode & S_IXOTH))
		puterr("cd: ", "permission denied: ", path);
	return (1);
}

/*
**  Builtin to output the passed arguments to stdout.
**
**  Synopsis: echo  [-n] [STRING]...
**
**  '-n': no trailing newline will be outputted.
**  Return 0 on succes and 1 on error.
*/

int						builtin_echo(const char **args)
{
	int					eol;

	if (*args && **args == '-' && (*args)[1] == 'n')
	{
		eol = 0;
		++args;
	}
	else
		eol = 1;
	while (*args)
	{
		ft_putstr(*args);
		if (*(args++ + 1))
			ft_putchar(' ');
	}
	if (eol)
		ft_putchar('\n');
	return (0);
}

/*
**  Builtin to print the working directory to stdout.
**
**  Synopsis: pwd
**
**  Return 0 on succes and 1 on error.
*/

int						builtin_pwd(const char **args)
{
	char				*buf;

	if (args && *args)
	{
		ft_putendl_fd("pwd: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (!(buf = getcwd(NULL, 0)))
	{
		ft_putendl_fd("pwd: an error has occured", STDERR_FILENO);
		return (1);
	}
	ft_putendl(buf);
	free(buf);
	return (0);
}

/*
**  Builtin to exit the shell.
**
**  Synopsis: exit [STATUS]
**
**  Exits with the specified status value.
**  If no status is specified, exits with EXIT_SUCCES as value.
**  Only the first argument will be taken into account, the other
**  ones will be ignored.
*/

void					builtin_exit(int status, char ***cmd, char ***env)
{
	if (cmd && *cmd)
		ft_strtabdel(cmd);
	if (env && *env)
		ft_strtabdel(env);
	exit(status);
}
