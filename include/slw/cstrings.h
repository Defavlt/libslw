#ifndef SLW_CSTRING_H
#define SLW_CSTRING_H

namespace SLW
{
	/* \brief Dynamic string copy
	 *     Copies `length` characters from `src`, starting at `start`.
	 * \param src the source string
	 * \param start the starting index
	 * \param lenght the length of the sub string
	 * \return a pointer to the sub string
	 * \note `Start + length < strlen(src)� required
	 * \note Remember to `free` your substring when done
	 */
	static inline char*
	strdcpy (
		const char* const src,
		const unsigned int start,
		const unsigned int length )
	{
		char* substr = ( char* ) malloc( 1 + length * sizeof( char ) );
		memcpy( substr, src + start, length + 1 );
		substr[ length ] = ( char )0;

		return substr;
	}
}

#endif//SLW_CSTRING_H
