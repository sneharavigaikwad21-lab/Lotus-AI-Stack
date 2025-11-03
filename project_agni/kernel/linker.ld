MEMORY
{
  RAM : ORIGIN = 0x30000000, LENGTH = 4K
}

SECTIONS
{
  .text :
  {
    *(.text)
  } > RAM
}
