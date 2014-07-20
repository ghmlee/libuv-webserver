#include <stdlib.h>
#include <stdio.h>

#include "uv.h"

static uv_loop_t* _uv_loop;
static uv_tcp_t _uv_server;

typedef struct {
  uv_tcp_t handle;
  uv_req_t write_req;
} client_t;

void tcp_close_cb(uv_handle_t* handle) {
  printf("disconnected\n");
  printf("%p\n", &handle->data);
  free(&handle->data);
}

void tcp_write_cb(uv_write_t* req, int status) {
  
}

void tcp_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  if (nread >= 0) {
    client_t* client = (client_t*)stream->data;

    uv_buf_t res_buf;
    res_buf.base = "Hello, world\n";
    res_buf.len = 13;
  
    uv_write((uv_write_t*)&client->write_req, (uv_stream_t*)&client->handle, &res_buf, 1, tcp_write_cb);
  } else {
    uv_close((uv_handle_t*)stream, tcp_close_cb);
  }

  free(buf->base);
}

void alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = malloc(suggested_size);
  buf->len = suggested_size;
}

void tcp_new_connection_cb(uv_stream_t* uv_server, int status) {
  if (uv_server != (uv_stream_t*)&_uv_server) return;
  if (status != 0) return;

  printf("connected\n");

  client_t* client = malloc(sizeof(client_t));  
  uv_tcp_init(_uv_loop, &client->handle);
  printf("%p\n", client);

  int error = uv_accept((uv_stream_t*)&_uv_server, (uv_stream_t*)&client->handle);
  if (error == 0) {
    client->handle.data = client;
    
    uv_read_start((uv_stream_t*)&client->handle, alloc_buffer_cb, tcp_read_cb);
  } else {
    uv_close((uv_handle_t*)&client->handle, tcp_close_cb);
  }
}

int main(int argc, char *argv[]) {
  struct sockaddr_in address;
  uv_ip4_addr("0.0.0.0", 8000, &address);

  _uv_loop = uv_default_loop();
  uv_tcp_init(_uv_loop, &_uv_server);
  uv_tcp_bind(&_uv_server, (const struct sockaddr*)&address, 0);
  uv_listen((uv_stream_t*)&_uv_server, 128, tcp_new_connection_cb);

  return uv_run(_uv_loop, UV_RUN_DEFAULT);
}
