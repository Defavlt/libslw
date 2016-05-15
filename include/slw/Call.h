#ifndef SLW_CALL_H
#define SLW_CALL_H

namespace SLW
{
	class State;

	class Call
	{
	public:

		/* \brief construct a new SLW::Call by
		 *   using the supplied function name
		 * \note Lua copies strings internally, so the string is yours once
		 *   control leaves.
		 */
		Call( SLW::State& state, const char* fn );

		/* \brief construct a new SLW::Call by
		 *   using the supplied index
		 */
		Call( SLW::State& state, const int index );

		virtual ~Call( void );

		/* \brief reset the parameter count of this call
		 * \note currently does not pop any values, use with care!
		 */
		Call&
		clear( void );

		/* \brief push a number unto the stack
		 */
		template< typename NumT >
		Call&
		param( const NumT v )
		{
			lua_pushnumber( state.state, v );
			++args;

			return *this;
		}

		/* \brief push a string unto the stack
		 * \note Lua copies strings internally, so the string is yours once
		 *   control leaves.
		 */
		Call&
		param( const char* v );

		/* \brief increment the number of pushed parameters
		 * \note useful if you push values unto the stack manually
		 */
		Call&
		param( void );

		/* \brief pop all arguments from the stack and call the 
		 * initially supplied function.
		 * \param [optional] nresults number of results to expect
		 * \return true if no errors (syntax or others) happened
		 * \note Define LUA_AS_CPP to catch (and re-throw) exceptions
		 */
		bool
		call( const unsigned int nresults = 0 );

	private:
		const Call& operator=( const Call& );

		int call_ref;
		unsigned int args;
		SLW::State& state;
		bool function_found;
	};
}

#endif//SLW_CALL_H