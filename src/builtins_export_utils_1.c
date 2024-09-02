#include "../include/minishell.h"

/*
allocate_env_var создает новую строку, т.е. переменную окружения в формате "ключ=значение".
size_t new_var_size;
переменная для хранения размера новой строки (ключ + '=' + значение переменной окружения).
char *new_env_var;
Указатель на новую строку, представляющую переменную окружения после выделения памяти под неё.
ft_strlcpy(new_env_var, key, ft_strlen(key) + 1);
Key копируется в начало new_env_var, включая '\0'.
new_env_var[ft_strlen(key)] = '=';
На следующей позиции после ключа (ft_strlen(key)) добавляем '='.
ft_strlcpy(new_env_var + ft_strlen(key) + 1, value, ft_strlen(value) + 1);
После '=', в оставшуюся часть строки копируем value, начиная с позиции ft_strlen(key) + 1.
( + '\0')

update_env_var обновляет существующую переменной окружения в структуре env_struct.
t_env *env_struct - указатель на структуру t_env, которая содержит массив переменных окружения.
Вызываем allocate_env_var, которая создает новую строку в формате "ключ=значение"
и возвращает указатель на неё. Эта строка будет хранить новое значение переменной окружения.
Если allocate_env_var вернет NULL, то выделение памяти не удалось, и переменная окружения
не может быть обновлена.
Если new_env_var равен NULL, то память не была выделена, и функция возвращает 0, что указывает
на ошибку.
free(env_struct->env[i]);
Если память успешно выделена, освобождаем старую строку, которая хранит текущее значение
переменной окружения в массиве env по индексу i.
env_struct->env[i] = new_env_var;
Обновляем указатель в массиве env по индексу i на новый адрес, где теперь хранится обновленная
строка переменной окружения в формате "ключ=значение"

update_existing_variable обновляет значение существующей переменной окружения
Если переменная найдена, меняем ее значение, функция возвращает 1.
Если переменная не найдена, функция возвращает 0.
t_env *env_struct - указатель на структуру t_env, которая содержит массив переменных окружения.
const char *key: ключ (имя переменной окружения), который нужно обновить.
const char *value: новое значение для переменной окружения.
key_len = ft_strlen(key);
Вычисление длины ключа, чтобы потом сравнивать строки на соответствие ключу.
while (env_struct->env[i])
Перебираем массив строк env пока не будет найдено совпадение с ключом или до конца массива.
if (ft_strncmp(env_struct->env[i], key, key_len) == 0 && env_struct->env[i][key_len] == '=')
Проверяем начинается ли строка env_struct->env[i] с ключа key, и следует ли сразу за ним "="
ft_strncmp(env_struct->env[i], key, key_len) == 0
проверяем совпадают ли key_len символы с ключом key. Если совпадают, то функция возвращает 0.
return (update_env_var(env_struct, i, key, value));
Если if выполняется, вызывается функция update_env_var, которая обновляет существующую
переменную окружения на новую, в формате key=value.
Если переменная окружения не была найдена на текущей итерации, инкрементируем i и повторяем цикл.
*/

char *allocate_env_var(const char *key, const char *value)
{
    size_t new_var_size;
    char *new_env_var;

    new_var_size = ft_strlen(key) + ft_strlen(value) + 2; // +1 для '=' и +1 для '\0'
    new_env_var = malloc(new_var_size);
    if (!new_env_var)
        return (NULL);
    ft_strlcpy(new_env_var, key, ft_strlen(key) + 1);
    new_env_var[ft_strlen(key)] = '=';
    ft_strlcpy(new_env_var + ft_strlen(key) + 1, value, ft_strlen(value) + 1);
    return (new_env_var);
}

int update_env_var(t_env *env_struct, int i, const char *key, const char *value)
{
    char *new_env_var;

    new_env_var = allocate_env_var(key, value);
    if (!new_env_var)
        return (0);
    free(env_struct->env[i]);
    env_struct->env[i] = new_env_var;
    return (1);
}

int update_existing_variable(t_env *env_struct, const char *key, const char *value)
{
    int i;
    int key_len;
    
    i = 0;
    key_len = ft_strlen(key);
    while (env_struct->env[i])
    {
        if (ft_strncmp(env_struct->env[i], key, key_len) == 0 && env_struct->env[i][key_len] == '=')
            return (update_env_var(env_struct, i, key, value));
        i++;
    }
    return (0);
}