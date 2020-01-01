/**
 * Compile:
 *   clang++ -std=c++14 main.cpp
 * Usage:
 *   ./a.out testfile1
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>


typedef std::pair< std::int32_t, std::int32_t > coord_t;

struct step_t
{
	char direction; // U,D,L,R
	std::size_t steps;
};

std::vector< step_t > parse_steps( const std::string& line )
{
	std::istringstream sstream( line );
	std::string item;
	std::vector< step_t > ret;

	while ( std::getline( sstream, item, ',' ) )
		ret.push_back( step_t{
			item.at( 0 ),
			static_cast< std::size_t >( std::stoul( item.substr( 1 ) ) )
		} );

	return ret;
}

template< class Fn >
void traverse( const std::vector< step_t >& steps, Fn&& fn )
{
	std::int32_t x = 0, y = 0;
	for( auto&& turn: steps )
	{
		// Walk "backwards" (negative x or y) if left or down
		std::int32_t step_add =
			( turn.direction == 'D' || turn.direction == 'L' ) ? -1 : 1;

		for ( std::int32_t i = 0; i < turn.steps; ++i )
		{
			if ( turn.direction == 'D' || turn.direction == 'U' )
				y += step_add;
			else
				x += step_add;
			fn( x, y );
		}
	}
}

int main( int argc, char** argv )
{
	std::ifstream file( argv[ 1 ] );

	std::string first, second;
	std::getline( file, first );
	std::getline( file, second );

	auto steps1 = parse_steps( first );
	auto steps2 = parse_steps( second );

	std::set< coord_t > coords;

	// Traverse first wire, save coords
	traverse( steps1, [ &coords ]( std::int32_t x, std::int32_t y )
	{
		// Only save if not start position
		if ( !( x == 0 && y == 0 ) )
			coords.insert( std::make_pair( x, y ) );
	} );

	std::set< std::int32_t > matches;

	// Traverse second wire, match coords, save matches
	traverse( steps2, [ &coords, &matches ]( std::int32_t x, std::int32_t y )
	{
		if ( coords.count( std::make_pair( x, y ) ) )
			matches.insert( std::abs( x ) + std::abs( y ) );
	} );

	// std::set is ordered, the lowest value is first
	std::cout << *matches.begin( ) << std::endl;
}
