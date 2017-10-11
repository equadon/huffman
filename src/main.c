#include <argp.h>
#include <error.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

const char *argp_program_version =
    "huffman 0.1";
const char *argp_program_bug_address =
    "<niklas.persson@gmail.com>";

static char doc[] =
    "Huffman coding -- Encode/decode strings using Huffman coding.\
\vSome more documentation here.";

static char args_doc[] = "CMD [MESSAGE...]";

static struct argp_option options[] = {
    {"verbose",  'v', 0,       0, "Produce verbose output" },
    {"output",   'o', "FILE",  0, "Output bits to FILE" },
    {"input",     'i', "FILE",  0, "Read Huffman tree and data from FILE"},

    { 0 }
};

struct arguments
{
    char cmd;           /* cmd */
    char **words;         /* [words…] */
    int verbose;         /* ‘-s’, ‘-v’, ‘--abort’ */
    char *output_file;   /* file arg to ‘--output’ */
    char *input_file;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key) {
        case 'v':
            arguments->verbose = 1;
            break;
        case 'o':
            arguments->output_file = arg;
            break;
        case 'i':
            arguments->input_file = arg;
            break;
        case ARGP_KEY_NO_ARGS:
            argp_usage(state);
        case ARGP_KEY_ARG:
            // parse command
            arguments->cmd = arg[0];

            // parse words
            arguments->words = &state->argv[state->next];
            state->next = state->argc;

            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc  };

void encode(struct arguments *arguments, char *message)
{
    int tree_size = 0;
    int unique_letters = 0;

    if (arguments->input_file) {
        FILE *input = fopen(arguments->input_file, "r");

        if (input == NULL) error(10, 0, "ERROR LOADING INPUT FILE");

        fseek(input, 0L, SEEK_END);
        int size = ftell(input);
        rewind(input);

        free(message);
        message = malloc((size + 1) * sizeof(char));
        fread(message, sizeof(char), size, input);

        message[size] = '\0';

        fclose(input);
    }

    huffman_node_t *root = huffman_new_tree(message, &tree_size, &unique_letters);
    char **index = huffman_build_index(root);

    bit_array_t *bits = huffman_encode(index, message);
    bit_array_t *tree_bits = huffman_encode_tree(root, tree_size, unique_letters);

    int msg_len = bit_array_length(bits);
    int tree_len = bit_array_length(tree_bits);

    if (arguments->verbose) {
        printf("Message (%d bits): %s\n", 8 * (int) strlen(message), message);
        printf("Binary (%d bits):  ", msg_len); bit_array_print(bits); puts("");
        printf("Tree (%d bits):  ", tree_len); bit_array_print(tree_bits); puts("");
    }

    float percent = 1 - (msg_len + tree_len) / (float) (8 * strlen(message));
    printf("Compression: %.1f%%\n", 100 * percent);

    if (arguments->output_file) {
        printf("Writing tree and message bits to file: %s\n", arguments->output_file);

        FILE *file = fopen(arguments->output_file, "w");

        if (file == NULL)
            error(10, 0, "FAILED TO OPEN OUTPUT FILE");

        bit_array_write(tree_bits, file);
        bit_array_write(bits, file);

        fclose(file);
    }

    free(message);

    huffman_free(root, index);

    bit_array_free(bits);
    bit_array_free(tree_bits);
}

void decode(struct arguments *arguments, char *message)
{
    char *output = NULL;

    if (arguments->input_file) {
        printf("Reading Huffman tree from file: %s\n", arguments->input_file);

        FILE *file = fopen(arguments->input_file, "r");

        if (file == NULL)
            error(10, 0, "FAILED TO READ TREE FILE");

        bit_array_t *tree_bits = bit_array_read(file);
        bit_array_t *bits = bit_array_read(file);

        if (arguments->verbose) {
            printf("Tree:  "); bit_array_print(tree_bits); puts("");
            printf("Bits:  "); bit_array_print(bits); puts("");
        }

        huffman_node_t *root = huffman_build_tree(tree_bits);

        output = huffman_decode(root, bits);

        huffman_free(root, NULL);
        bit_array_free(tree_bits);
        bit_array_free(bits);

        fclose(file);
    }

    if (arguments->verbose) {
        printf("Output: %s\n", output);
    }

    if (arguments->output_file) {
        FILE *out_file = fopen(arguments->output_file, "w");

        fwrite(output, sizeof(char), strlen(output), out_file);

        fclose(out_file);
    }

    free(message);
    free(output);
}

int main(int argc, char **argv)
{
    struct arguments arguments;

    // Default values
    arguments.verbose = 0;
    arguments.output_file = NULL;
    arguments.input_file = NULL;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // concat message
    int length = 0;
    int num_words = 0;
    for (int i = 0; arguments.words[i]; i++) {
        length += 1 + strlen(arguments.words[i]);
        num_words++;
    }

    if (length == 0 && arguments.input_file == NULL) error(10, 0, "NO INPUT");

    int index = 0;
    char *message = calloc(length + 1, sizeof(char));
    for (int i = 0; arguments.words[i]; i++) {
        for (int j = 0; j < strlen(arguments.words[i]); j++)
            message[index++] = arguments.words[i][j];
        if (i < num_words - 1)
            message[index++] = ' ';
    }
    message[index] = '\n';

    // Parse commands
    switch (arguments.cmd) {
        case 'e':
            encode(&arguments, message);
            break;
        case 'd':
            decode(&arguments, message);
            break;
        default:
            error(10, 0, "UNKNOWN COMMAND");
    }

    return 0;
}
