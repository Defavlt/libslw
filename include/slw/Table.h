#ifndef SLW_TABLE_H
#define SLW_TABLE_H

namespace SLW
{
	class State;

	/* \brief Create or modify a table
	 *
	 **/
	class Table
	{
	public:

		Table( SLW::State& state, const int index = -1 );

		/* \brief allocate space for and create a new table
		 * \param narr number of array elements
		 * \param nrec number of non-array elements
		 *
		 **/
		SLW::Table&
		Allocate( const int narr, const int nrec );

		bool
		Find( const char* field, bool& result );

		/* \brief Set table element this[field] = v
		 **/
		template< typename VecT, unsigned int length >
		SLW::Table&
		Set( const VecT v )
		{
			for ( unsigned int i = 0; i < length; ++i )
			{
				state.push( v[ i ] );

				// remember: lua array fields start 1
				lua_rawseti( state.state, index - 1, i + 1 );
			}

			return *this;
		}

		template< typename TypeT >
		SLW::Table&
		Set( const char* field, TypeT& v)
		{
			state.push( v );
			lua_setfield( state.state, index, field );

			return *this;
		}

		/* \brief Get table element this[field]
		 * \note remember to free v if it's a string
		 **/
		template< typename VecT, unsigned int length >
		SLW::Table&
		Get( const char* field, VecT& v )
		{
			lua_getfield( state.state, index, field );

			int type = lua_type( state.state, index );

			if ( type == LUA_TTABLE )
			{
				lua_pushnil( state.state );

				int i = 0;

				/*
				* stack:
				*   1 nil
				*   2 table
				*/

				for ( ; /*i < length && */lua_next( state.state, index - 1 ); ++i )
				{
					/*
					* stack:
					*   1 value
					*   2 key
					*   3 table
					*/

					state.pop( v[ i ] );

					/*
					* stack:
					*   1 key
					*   2 table
					*/
				}
			}

			lua_pop( state.state, 1 );

			return *this;
		}

		template< typename TypeT >
		SLW::Table&
		Get( const char* field, TypeT& v )
		{
			lua_getfield( state.state, index, field );
			state.pop( v, 0, true );

			return *this;
		}

		template< typename VecT, unsigned int length >
		SLW::Table&
		Get( int field, VecT& v )
		{
			lua_rawgeti( state.state, index, field );

			int type = lua_type( state.state, index );

			if ( type == LUA_TTABLE )
			{
				lua_pushnil( state.state );

				int i = 0;

				/*
				* stack:
				*   1 nil
				*   2 table
				*/

				for ( ; lua_next( state.state, index - 1 ); ++i )
				{
					/*
					* stack:
					*   1 value
					*   2 key
					*   3 table
					*/

					state.pop( v[ i ] );

					/*
					* stack:
					*   1 key
					*   2 table
					*/
				}
			}

			lua_pop( state.state, 1 );

			return *this;
		}

		template< typename TypeT >
		SLW::Table&
		Get( int field, TypeT& v )
		{
			lua_rawgeti( state.state, index, field );
			state.pop( v, 0, true );

			return *this;
		}

	private:
		const SLW::Table& operator=( const SLW::Table& );

		const int index;

		SLW::State& state;
	};
}

#endif//SLW_TABLE_H