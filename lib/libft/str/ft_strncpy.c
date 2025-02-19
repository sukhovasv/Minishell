char	*ft_strncpy(char *dst, const char *src, unsigned int n)
{
	char *const	dest = dst;
	const char	*s = src;

	while (n-- != 0)
	{
		*dst++ = *s;
		if (*s++ == 0)
		{
			while (n-- != 0)
				*dst++ = '\0';
			break ;
		}
	}
	return (dest);
}