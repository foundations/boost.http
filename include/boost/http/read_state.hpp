/* Copyright (c) 2014 Vinícius dos Santos Oliveira

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) */

#ifndef BOOST_HTTP_READ_STATE_HPP
#define BOOST_HTTP_READ_STATE_HPP

namespace boost {
namespace http {

/**
 * Represents the current state in the HTTP incoming request or HTTP incoming
 * response.
 *
 * Be prepared to face multiple state changes after a single action is scheduled
 * (e.g. you issue `receive_message` action and the state already changed to
 * empty when the handler is invoked). However, the message producer (e.g. the
 * embedded server) is not allowed to change the state from `empty` to
 * `request_ready` without another iteraction from the library user.
 */
enum class read_state
{
    /**
     * This is the initial state.
     *
     * There are two ways to interpret this state. It might mean that the
     * request object wasn't used/read yet.
     *
     * Another interpretation is that it was reached from the `body_ready`
     * state, after all trailers have been received. It's safe to assume that
     * all message data is available if this is the case.
     *
     * An easy to interpret this state is that it is complete for the previous
     * message and empty for the next one.
     *
     * At this state, you can only issue a `receive_message` action.
     */
    empty,
    /**
     * This state is reached from the `empty` state, once you ask for a new
     * request.
     *
     * No more `receive_message` actions can be issued from this state.
     *
     * From this state, you can issue the `receive_some_body` action. The state
     * will change to `body_ready` once all body was read. In streaming
     * connections (e.g. HTTP/1.1 chunked entities), this condition (body fully
     * received) might never happen.
     *
     * Once this state is reached, you can safely read the first line and the
     * headers. */
    message_ready,
    /**
     * This state is reached from the `request_ready`, once the http producer
     * (e.g. embedded server) fully received the message body.
     *
     * From this state, you can only issue the `receive_trailers` action.
     *
     * Once this state is reached, you can safely assume that no more body parts
     * will be received.
     */
    body_ready,
    /**
     * Only possible in incoming response mode. It means the message is complete
     * and you can no longer issue another `receive_message` until something
     * else is done (e.g. send another http request). This is a
     * different/special value, because the "something else to do" might not be
     * related to read actions.
     *
     * It can be reached from `body_ready` state, after all trailers have been
     * received. It's safe to assume that all message data is available at the
     * time this state is reached.
     */
    finished
};

} // namespace http
} // namespace boost

#endif // BOOST_HTTP_READ_STATE_HPP
