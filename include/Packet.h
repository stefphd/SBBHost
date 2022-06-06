
#ifndef PACKET_H
#define PACKET_H

#include "config.h"
#include <string>

/*! \brief A templated-class for managing data packet.
    \details Templated-class for managing data-packet with used-defined type and size, used in SBBHostCore.
	\see SBBHostCore
	\tparam T The base type of the packet.
	\tparam Size The size of the packet (number of element with type T).
    \author Stefano Lovato
    \date 2022
*/
template<typename T, size_t Size>
class Packet {
public:
	T value[Size] = { 0 }; //!< Buffer with base type T and size Size.

	unsigned int id[Size]; //!< Zero-based index of data elements (from 0 to Size-1).

	const unsigned int size = Size; //!< Number of element of the packet.

	const unsigned int bytes = Size * sizeof(T); //!< Number of bytes of the packet according to sizeof(T) and Size.

	/*! \brief Constructor.
		\details Default constructor.
	*/
	Packet() { for (int i = 0; i < size; i++) id[i] = i; };

	/*! \brief Get a value of the packet.
		\details Function to get a value of the packet.
		\param i The index of the value.
		\return The value of type T.
	*/
	T getValue(unsigned int i) { return value[i]; };
};

#endif
