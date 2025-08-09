/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_PATH 4096
# define MAX_LINE 1024
# define MAX_ARGS 1024

extern int	g_signal;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_REDIR_APPEND,
	NODE_REDIR_HEREDOC
}	t_node_type;

typedef struct s_redir
{
	t_node_type			type;
	char				*file;
	int					fd;
	struct s_redir		*next;
}	t_redir;

typedef struct s_cmd
{
	char				**args;
	t_redir				*redirs;
}	t_cmd;

typedef struct s_ast
{
	t_node_type			type;
	t_cmd				*cmd;
	struct s_ast		*left;
	struct s_ast		*right;
}	t_ast;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}	t_env;

typedef struct s_shell
{
	t_env				*env;
	int					exit_code;
	int					stdin_fd;
	int					stdout_fd;
}	t_shell;

/* Tokenizer functions */
t_token		*tokenize(char *input);
void		free_tokens(t_token *tokens);
t_token		*create_token(t_token_type type, char *value);
void		add_token(t_token **tokens, t_token *new_token);
char		*extract_quoted_string(char **input, char quote);
t_token_type	get_operator_type(char **input);

/* Parser functions */
t_ast		*parse(t_token *tokens);
void		free_ast(t_ast *ast);
t_ast		*create_ast_node(t_node_type type);
t_redir		*create_redir(t_node_type type, char *file);
t_cmd		*create_cmd(void);
void		add_redir_to_list(t_redir **redirs, t_redir *new_redir);
void		push_arg(char **args, int *arg_count, char *value);
void		handle_redirection_token(t_token **current, t_redir **redirs);
int			alloc_cmd_and_args(t_ast **node, char ***args);
void		process_token(t_token **current, char ***args,
			int *arg_count, t_redir **redirs);

/* Environment functions */
t_env		*init_env(char **envp);
char		*get_env_value(t_env *env, char *key);
t_env		*set_env_value(t_env *env, char *key, char *value);
t_env		*unset_env_value(t_env *env, char *key);
void		free_env(t_env *env);
char		**env_to_array(t_env *env);

/* Expansion functions */
char		*expand_variables(char *str, t_shell *shell);
char		*expand_single_var(char *str, int *pos, t_shell *shell);

/* Execution functions */
int			execute_ast(t_ast *ast, t_shell *shell);
int			execute_command(t_ast *ast, t_shell *shell);
int			execute_pipeline(t_ast *ast, t_shell *shell);

/* Built-in functions */
int			is_builtin(char *cmd);
int			execute_builtin(char **args, t_shell *shell);
int			builtin_echo(char **args);
int			builtin_cd(char **args, t_shell *shell);
int			builtin_pwd(void);
int			builtin_env(t_shell *shell);
int			builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);
int			builtin_exit(char **args, t_shell *shell);

/* Redirection functions */
int			open_input_file(char *filename);
int			open_output_file(char *filename, int append);
int			handle_heredoc(char *delimiter);
int			setup_redirections(t_redir *redirs);
void		restore_redirections(int stdin_fd, int stdout_fd);

/* Signal functions */
void		setup_signals(void);
void		handle_sigint(int sig);

/* Utility functions */
char		*ft_strdup(const char *s);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		**ft_split(char const *s, char c);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
size_t		ft_strlen(const char *s);
char		*ft_strchr(const char *s, int c);
char		*ft_strtrim(char const *s1, char const *set);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		free_array(char **arr);

/* Error functions */
void		error_exit(char *msg);
void		print_error(char *cmd, char *msg);

#endif