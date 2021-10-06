#include "Connection.h"

Connection::Connection()
	: m_id(newID++)
{}

void Connection::AddConfig(Config config) 
{
	m_config = config;
}

unsigned int Connection::GetID()
{
	return m_id;
}