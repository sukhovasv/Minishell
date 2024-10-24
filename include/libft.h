/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yberezhn <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 12:31:37 by yberezhn          #+#    #+#             */
/*   Updated: 2023/11/07 14:42:22 by yberezhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdarg.h>
# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

struct	s_dlnode;

typedef struct s_dllist
{
	size_t			size;
	struct s_dlnode	*head;
	struct s_dlnode	*tail;
}				t_dllist;

//TODO int to long, num -> val
typedef struct s_dlnode
{
	int				num;
	int				cost;
	struct s_dlnode	*next;
	struct s_dlnode	*prev;
	struct s_dlnode	*target;
	t_dllist		*list;

}				t_dlnode;

int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
size_t		ft_strlen(const char *s);
void		*ft_memset(void *s, int c, size_t n);
void		ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);
int			ft_atoi(const char *nptr);

void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_strdup(const char *s);
char		*ft_strndup(const char *source, size_t n);

char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);

bool		is_int(char *arg);
int			str_arr_len(char **arr);
void		free_arr_el(char **arr, int len);

t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

char		*get_next_line(int fd);
int			ft_printf(char const *s, ...);

int			flag_handler(char flag, va_list *args);

int			ft_print_char(int c);
int			ft_print_str(char *str);
int			ft_print_num(char flag, va_list *args);
int			ft_print_p(va_list *args);

char		*ft_itoa_base(int n, const char *base, int radix);
char		*ft_ultoa_base(unsigned long n, const char *base, int radix);

t_dllist	*initializeList(void);
t_dlnode	*dlnode_alloc(int num);
t_dlnode	*dllist_free(t_dlnode *list);
size_t		dllist_length(t_dlnode *list);
int			dllist_node_idx(t_dllist *list, t_dlnode *node);
t_dlnode	*dllist_last(t_dlnode *list);
t_dlnode	*dllist_append(t_dlnode *list, int num);
t_dllist	*dllist_append_node(t_dllist *list, t_dlnode *node);
t_dllist	*dllist_prepend_node(t_dllist *list, t_dlnode *node);
t_dlnode	*dllist_prepend(t_dlnode *list, int num);

t_dlnode	*dllist_detach_node(t_dllist *list, t_dlnode *link);
void		dllist_foreach(t_dlnode *list,
				void (*f)(void *, void *), void *user_data);
void		dllist_foreach_el(t_dlnode *list,
				void (*f)(void *, void *), void *user_data);

#endif //LIBFT_H
