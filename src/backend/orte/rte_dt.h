#include <unistd.h>

#ifndef RTE_DT_H
#define RTE_DT_H
struct rte_dt_t {
    int opal_dt;
};

size_t get_datatype_size(const struct rte_dt_t *dt);
#endif
