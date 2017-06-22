#ifndef SYNCHRONIZATION_HPP
#define SYNCHRONIZATION_HPP

#include <functional>

void lock_mocks_enable(bool enabled);
void condvar_mocks_enable(bool enabled);
void condvar_init_mock_enable(bool enabled);
void condvar_mocks_ignore_cv_pointer_arg(bool enabled);

void set_condvar_wait_side_effect(std::function<void()> side_effect);
void clear_condvar_wait_side_effect();

#endif
