MEMORY
{
  RAM : ORIGIN = 0x20000000, LENGTH = 4K
}

SECTIONS
{
  .text :
  {
    *(.text)
  } > RAM
}
