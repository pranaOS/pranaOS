/**
 * @file devicetree.h
 * @author Krisna Pranav
 * @brief device tree
 * @version 6.0
 * @date 2025-06-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#define dt_u32(x) be32toh(x)
#define dt_u64(x) be64toh(x)
#define dt_string(b, x) dtb_string(b, dt_u32(x))
#define dt_get_prop_value(b, n, nm) dtb_read_value((b), (n), dtb_get_prop((b), (n), (nm)))

#define dt_reg_foreach(ri, r) \
    for (range r; dtb_reg_iterate(&(ri), &(r));)

enum {
    DT_VALUE_INVALID,
    DT_VALUE_UNKNOWN,
    DT_VALUE_EMPTY,
    DT_VALUE_U32,
    DT_VALUE_U64,
    DT_VALUE_STRING,
    DT_VALUE_PHANDLE,
    DT_VALUE_STRINGLIST,
    DT_VALUE_REG,
    DT_VALUE_FREQ,
};

typedef struct dt_node *dt_node;
typedef struct dt_prop *dt_prop;

typedef struct dt_reg_iterator {
    u32 *data_ptr;
    u32 *data_end;
    u32 address_cells;
    u32 size_cells;
} dt_reg_iterator;

typedef struct dt_value {
    int type;
    void *data;
    u32 dlen;
    union {
        u64 num;
        char *string;
        dt_reg_iterator ri;
    } u;
} dt_value;

/**
 * @brief Construct a new closure type object
 * 
 * @param n 
 * @param name 
 */
closure_type(dt_node_handler, boolean, dt_node n, sstring name);

/**
 * @param prop 
 * @return unsigned int 
 */
unsigned int dt_prop_cell_count(dt_prop prop);

/**
 * @param prop 
 * @param index 
 * @return u32 
 */
u32 dt_prop_get_cell(dt_prop prop, unsigned int index);

typedef struct fdt {
    void *ptr, *end;
    char *strings_start, *strings_end;
} *fdt;

/**
 * @param dtb 
 * @param off 
 * @return sstring 
 */
sstring dtb_string(void *dtb, u64 off);

/**
 * @param p 
 * @return u32 
 */
u32 dtb_read_u32(dt_prop p);

/**
 * @param p 
 * @return u64 
 */
u64 dtb_read_u64(dt_prop p);

/**
 * @param dtb 
 * @param n 
 * @param p 
 * @return dt_reg_iterator 
 */
dt_reg_iterator dtb_read_reg(void *dtb, dt_node n, dt_prop p);

/**
 * @param ri 
 * @param r 
 * @return boolean 
 */
boolean dtb_reg_iterate(dt_reg_iterator *ri, range *r);

/**
 * @param dtb 
 * @return range 
 */
range dtb_read_memory_range(void *dtb);

/**
 * @param dtb 
 * @param path 
 * @return dt_node 
 */
dt_node dtb_find_node_by_path(void *dtb, sstring path);

/**
 * @param dtb 
 * @param phandle 
 * @return dt_node 
 */
dt_node dtb_find_node_by_phandle(void *dtb, u32 phandle);

/**
 * @param dtb 
 * @param n 
 * @param match 
 * @param nh 
 * @return int 
 */
int dtb_walk_node_children(void *dtb, dt_node n, sstring match, dt_node_handler nh);

/**
 * @param dtb 
 * @return u32 
 */
u32 dtb_blob_size(void *dtb);

/**
 * @param dtb 
 * @return dt_node 
 */
dt_node dtb_get_root(void *dtb);

/**
 * @param dtb 
 * @param n 
 * @return dt_node 
 */
dt_node dtb_get_parent(void *dtb, dt_node n);

/**
 * @param dtb 
 * @param n 
 * @param pname 
 * @return dt_prop 
 */
dt_prop dtb_get_prop(void *dtb, dt_node n, sstring pname);

/**
 * @param dtb 
 * @param n 
 * @param p 
 * @return dt_value 
 */
dt_value dtb_read_value(void *dtb, dt_node n, dt_prop p);

/**
 * @param dtb 
 */
void devicetree_dump(void *dtb);

/**
 * @param dtb 
 * @param fdt 
 * @return boolean 
 */
boolean dtb_parse_init(void* dtb, fdt fdt);

/**
 * @param fdt 
 * @return dt_node 
 */
dt_node fdt_get_node(fdt fdt);

/**
 * @param fdt 
 * @return dt_node 
 */
dt_node fdt_next_node(fdt fdt);

/**
 * @param fdt 
 * @param node 
 * @return sstring 
 */
sstring fdt_node_name(fdt fdt, dt_node node);

/**
 * @param fdt 
 * @param acells 
 * @param scells 
 */
void fdt_get_cells(fdt fdt, u32 *acells, u32 *scells);

/**
 * @param fdt 
 * @param acells 
 * @param scells 
 * @param iter 
 * @return boolean 
 */
boolean fdt_get_reg(fdt fdt, u32 acells, u32 scells, dt_reg_iterator *iter);

/**
 * @param fdt 
 * @param name 
 * @return dt_prop 
 */
dt_prop fdt_get_prop(fdt fdt, sstring name);

#define fdt_foreach_node(fdt, node) \
    for (dt_node node = fdt_get_node(fdt); node; node = fdt_next_node(fdt))