#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h> // intptr_t
#include <stdlib.h>

/**
 * @see http://www.w3.org/TR/html5/introduction.html
 * @see http://www.w3.org/TR/html5/dom.html#the-document-object
 *
 * @see http://www.w3.org/TR/html5/webappapis.html#webappapis
 *
 * @see http://www.w3.org/TR/html5/semantics.html#semantics
 * @see https://en.wikipedia.org/wiki/HTML_element#Elements_vs._tags
 *
 * @see https://developer.mozilla.org/en-US/docs/Web/API/HTMLElement
 *
 * ```idl
 * interface HTMLElement : Element {
 *   // metadata attributes
 *            attribute DOMString title;
 *            attribute DOMString lang;
 *            attribute boolean translate;
 *            attribute DOMString dir;
 *   readonly attribute DOMStringMap dataset;
 *
 *   // user interaction
 *            attribute boolean hidden;
 *   void click();
 *            attribute long tabIndex;
 *   void focus();
 *   void blur();
 *            attribute DOMString accessKey;
 *   readonly attribute DOMString accessKeyLabel;
 *            attribute boolean draggable;
 *   [PutForwards=value] readonly attribute DOMSettableTokenList dropzone;
 *            attribute DOMString contentEditable;
 *   readonly attribute boolean isContentEditable;
 *            attribute boolean spellcheck;
 * };
 *
 * HTMLElement implements GlobalEventHandlers;
 *
 * interface HTMLUnknownElement : HTMLElement { };
 * ```
 * @see http://www.w3.org/TR/html5/dom.html#htmlelement
 *
 * @see https://developer.mozilla.org/en-US/docs/Web/API/DOMString
 *
 * > Note also that null is not a value of type DOMString. To allow null, a
 * > nullable DOMString, written as DOMString? in IDL, needs to be used.
 * @see https://heycam.github.io/webidl/#idl-DOMString
 *
 * ISSUE-42: RDFa API should use 'string' instead of 'DOMString' in the WebIDL
 * ===========================================================================
 * @see http://www.w3.org/2010/02/rdfa/track/issues/42
 *
 * PROPOSAL to close ISSUE-42: DOMString vs. string
 * ================================================
 * @see http://lists.w3.org/Archives/Public/public-rdfa-wg/2010Sep/0023.html
 *
 * > 'For Java and ECMAScript, DOMString is bound to the String type because
 * > both languages also use UTF-16 as their encoding.'
 * @see http://lists.w3.org/Archives/Public/public-rdfa-wg/2010Aug/0077.html
 *
 * @see http://www.w3.org/TR/DOM-Level-3-Core/core.html#ID-C74D1578
 *   [and it's child]
 * @see http://www.w3.org/TR/DOM-Level-3-Core/core.html#DOMString
 *
 * @see http://tools.ietf.org/html/rfc2781    # <- UTF-16 encoding of ISO 10646
 *                                            #    The above is what "DOMString"
 *                                            #    corresponds to.
 *
 * @see http://tools.ietf.org/html/rfc3629    # <- UTF-8 encoding of ISO 10646
 *
 * @see www.w3.org/TR/html5/syntax.html
 *
 * @see https://en.wikipedia.org/wiki/IDL_%28programming_language%29
 *
 * In this quick mockup psuedo-code we're using a char* for DOMString types.
 * The latter being a UTF-16 object, but I don't believe we'll need to use that
 * (UTF-16 vs. UTF-8 and/or ASCII) internally here.
 *
 * ```c
 * typedef struct {
 *     char *title;
 *     char *lang;
 *     bool translate;
 *     char *dir;
 *     // readonly attribute DOMStringMap dataset; # <-- IDK, hash table maybe?
 * } HTMLElement;
 * ```
 *
 * @see http://www.w3.org/TR/html5/infrastructure.html#valid-month-string
 *
 * ```
 * 10.3 Non-replaced elements
 *
 *   10.3.1 Hidden elements
 *
 * @namespace url(http://www.w3.org/1999/xhtml);
 *
 * [hidden], area, base, basefont, datalist, head, link,
 * meta, noembed, noframes, param, rp, script, source, style, template, track, title {
 *   display: none;
 * }
 *
 * embed[hidden] { display: inline; height: 0; width: 0; }
 * ```
 * @see www.w3.org/TR/html5/rendering.html#non-replaced-elements
 */

// -----------------------------------------------------------
// >   *p+ + = val; /* push val onto stack */
// >   val = *--p;  /* pop top of stack into val */
// >
// > are the standard idioms for pushing and popping a stack;
//
//       -- Kernighan, Brian W.; Ritchie, Dennis (1988-03-22).
//              C Programming Language (2nd Edition) (p. 106).
//                              Prentice Hall. Kindle Edition.
// -----------------------------------------------------------

typedef enum elements {
    a,
    abbr,
    address,
    area,
    article,
    aside,
    audio
} ELEMENTS;

#define ELEMENTS_FIRST a
#define ELEMENTS_LAST  audio

// man sprintf    # @see https://en.wikipedia.org/wiki/Printf_format_string
// man 3 printf_l
// man 3 xlocale  # @see https://en.wikipedia.org/wiki/Locale
// man format     # <- This is for Tkinter but it is still relavent

#define printf_fmt(N)           \
    _Generic((N),               \
        char:           "%c",   \
        signed char:    "%hhd", \
        unsigned char:  "%hhu", \
        char *:         "%s",   \
        float:          "%f",   \
        int:            "%d",   \
        long:           "%ld",  \
        unsigned long:  "%lu",  \
        short:          "%hd",  \
        unsigned short: "%hu",  \
        void *:         "%p"    \
    )

#define nputs(n) printf(printf_fmt(n), n), printf("\n");

// FYI, you can use "%d" for general numeric values in theory but it is probably
// sub-optimal.

int main(void)
{
    printf("%lu\n", sizeof(ELEMENTS));                // 4
    printf("%d\n",  ELEMENTS_LAST - ELEMENTS_FIRST);  // 6
    printf("%d\n",  ELEMENTS_LAST);                   // 6
    printf("%d\n",  ELEMENTS_LAST + 1);               // 7 <- Correct!!! :D
    //              ^ We take the numeric value of the last element and add one
    //              to account for the zero index.

    unsigned short us = 1;

    nputs(us);

    return 0;
}
