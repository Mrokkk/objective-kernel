#pragma once

void *operator new(std::size_t, void *address);
void *operator new[](std::size_t, void *address);

