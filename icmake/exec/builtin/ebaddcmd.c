/*
\funcref{eb_addCmd}{char $**$eb_addCmd (\params)}
    {
        {char} {**cmd} {{\em argv}-like array of strings}
        {char} {*string} {string to add to {\em cmd}}
    }
    {resized {\em cmd} to which reference to {\em string} is added}
    {rss_realloc()}
    {}
    {eb_addCmd.c}
    {

        Function {\em eb_addCmd()} expects a pointer to an array of {\em char $*$}'s as
        first argument. This pointer may be {\em NULL}. The {\em argv}-like
        array is expanded to hold a reference to {\em string}.

        The reallocated array {\em cmd} is returned. The last field in the
        array is {\em NULL} to indicate the ending of the list.

        {\bf Note that} a reference to {\em string} itself is not added to the
        list: a duplicate is made.

    }
*/

#include "builtin.ih"

char **eb_addCmd(char **cmd, char *string)
{
    register int
        i,
        size = 0;

    if (cmd)
        for (i = 0; cmd [i]; i++)
            size++;

    cmd = rss_realloc (cmd, (size + 2) * sizeof (char *));
    cmd [size] = rss_strdup (string);
    cmd [size + 1] = NULL;

    return (cmd);
}