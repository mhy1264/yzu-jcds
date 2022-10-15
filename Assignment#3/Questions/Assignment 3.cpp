#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;

#include <set>
#include "set.h"

template< typename T, typename T1 = less< T >, typename T2 = std::less< T > >
void testSet();

// returns true if and only if set1 == set2
template< typename T, typename T1, typename T2 >
bool equal( set< T, T1 > &set1, std::set< T, T2 > &set2 );

// returns true if and only if 
// the subtree rooted at node1 is equal to the subtree rooted at node2
template< typename Kty >
bool equal( size_t *node1, size_t *head1,
            size_t *node2, size_t *head2 );

int main()
{
   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< char >();
   }
   cout << endl;

   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< char, greater< char >, std::greater< char > >();
   }
   cout << endl;

   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< short >();
   }
   cout << endl;

   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< short, greater< short >, std::greater< short > >();
   }
   cout << endl;

   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< long >();
   }
   cout << endl;

   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< long, greater< long >, std::greater< long > >();
   }
   cout << endl;

   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< long long >();
   }
   cout << endl;

   for( unsigned int seed = 11; seed <= 20; seed++ )
   {
      srand( seed );
      testSet< long long, greater< long long >, std::greater< long long > >();
   }
   cout << endl;

   system( "pause" );
}

template< typename T, typename T1, typename T2 >
void testSet()
{
   unsigned int numKeys = 1000;
   unsigned int key;

      set< T, T1 > set1;
      std::set< T, T2 > set2;

      unsigned int numErrors = numKeys + 1;
      if( equal( set1, set2 ) )
         numErrors--;

      for( unsigned int i = 0; i < numKeys; i++ )
      {
         switch( rand() % 3 )
         {
         case 0:
         case 1:
            key = 1 + rand() % ( 5 * numKeys );
            set1.insert( key );
            set2.insert( key );
            break;
         case 2:
            if( set2.size() > 0 )
            {
               key = 1 + rand() % ( 5 * numKeys );
               set1.erase( key );
               set2.erase( key );
            }
         }

         if( equal( set1, set2 ) )
            numErrors--;
      }

      cout << "There are " << numErrors << " errors.\n";
}

unsigned long long int twoTo32 = static_cast< unsigned long long int >( ULONG_MAX ) + 1;
unsigned long long int twoTo40 = twoTo32 * 256;
unsigned long long int twoTo48 = twoTo32 * 65536;

// returns true if and only if set1 == set2
template< typename T, typename T1, typename T2 >
bool equal( set< T, T1 > &set1, std::set< T, T2 > &set2 )
{
#if _ITERATOR_DEBUG_LEVEL == 2 // Solution Configuration is Debug
   size_t offset1 = 1;
   size_t offset2 = 2;
#else                        // Solution Configuration is Release
   size_t offset1 = 0;
   size_t offset2 = 1;
#endif

   size_t size1 = *( reinterpret_cast< size_t * >( &set1 ) + 2 );
   size_t size2 = *( reinterpret_cast< size_t * >( &set2 ) + offset2 );

   if( size1 != size2 )
      return false;

   size_t *head1 = *( reinterpret_cast< size_t ** >( &set1 ) + 1 );
   size_t *head2 = *( reinterpret_cast< size_t ** >( &set2 ) + offset1 );

   char color1 = static_cast< char >( *( head1 + 3 ) % 256 );
   char color2 = static_cast< char >( *( head2 + 3 ) % 256 );

   if( color1 != color2 )
      return false;

   char isNil1 = static_cast< char >( *( head1 + 3 ) / 256 % 256 );
   char isNil2 = static_cast< char >( *( head2 + 3 ) / 256 % 256 );

   if( isNil1 != isNil2 )
      return false;

   if( size1 == 0 )
      return true;

   size_t *root1 = *( reinterpret_cast< size_t ** >( head1 ) + 1 );
   size_t *root2 = *( reinterpret_cast< size_t ** >( head2 ) + 1 );
   return equal< T >( root1, head1, root2, head2 );
}

// returns true if and only if 
// the subtree rooted at node1 is equal to the subtree rooted at node2
template< typename Kty >
bool equal( size_t *node1, size_t *head1, size_t *node2, size_t *head2 )
{
   // node1 == head1 if and only if node1 is an external node
   // node2 == head2 if and only if node2 is an external node
   if( node1 == head1 && node2 == head2 )
      return true;

   if( node1 == head1 && node2 != head2 )
      return false;

   if( node1 != head1 && node2 == head2 )
      return false;

   char color1 = static_cast< char >( *( node1 + 3 ) % 256 );
   char color2 = static_cast< char >( *( node2 + 3 ) % 256 );
   if( color1 != color2 )
      return false;

   char isNil1 = static_cast< char >( *( node1 + 3 ) / 256 % 256 );
   char isNil2 = static_cast< char >( *( node2 + 3 ) / 256 % 256 );
   if( isNil1 != isNil2 )
      return false;

   Kty myVal1;
   Kty myVal2;
   if( sizeof( size_t ) == 8 ) // x64 platform
   {
      switch( sizeof( Kty ) )
      {
      case 1:
         myVal1 = static_cast< Kty >( *( node1 + 3 ) / 65536 % 256 );
         myVal2 = static_cast< Kty >( *( node2 + 3 ) / 65536 % 256 );
         break;
      case 2:
         myVal1 = static_cast< Kty >( *( node1 + 3 ) / 65536 % 65536 );
         myVal2 = static_cast< Kty >( *( node2 + 3 ) / 65536 % 65536 );
         break;
      case 4:
         myVal1 = static_cast< Kty >( *( node1 + 3 ) / twoTo32 );
         myVal2 = static_cast< Kty >( *( node2 + 3 ) / twoTo32 );
         break;
      case 8:
         myVal1 = static_cast< Kty >( *( node1 + 4 ) );
         myVal2 = static_cast< Kty >( *( node2 + 4 ) );
      }
   }
   else // x86 platform
   {
      switch( sizeof( Kty ) )
      {
      case 1:
         myVal1 = static_cast< Kty >( *( node1 + 3 ) / 65536 % 256 );
         myVal2 = static_cast< Kty >( *( node2 + 3 ) / 65536 % 256 );
         break;
      case 2:
         myVal1 = static_cast< Kty >( *( node1 + 3 ) / 65536 );
         myVal2 = static_cast< Kty >( *( node2 + 3 ) / 65536 );
         break;
      case 4:
         myVal1 = static_cast< Kty >( *( node1 + 4 ) );
         myVal2 = static_cast< Kty >( *( node2 + 4 ) );
         break;
      case 8:
         myVal1 = static_cast< Kty >( *( node1 + 4 ) );
         myVal2 = static_cast< Kty >( *( node2 + 4 ) );
      }
   }

   if( myVal1 != myVal2 )
      return false;

   size_t *leftChild1 = *( reinterpret_cast< size_t ** >( node1 ) );
   size_t *rightChild1 = *( reinterpret_cast< size_t ** >( node1 ) + 2 );
   size_t *leftChild2 = *( reinterpret_cast< size_t ** >( node2 ) );
   size_t *rightChild2 = *( reinterpret_cast< size_t ** >( node2 ) + 2 );

   return equal< Kty >( leftChild1, head1, leftChild2, head2 ) &&
          equal< Kty >( rightChild1, head1, rightChild2, head2 );
}