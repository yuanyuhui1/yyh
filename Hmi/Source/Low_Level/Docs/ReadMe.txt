Low Level File Structure...

Low Level Project Route Directory
 ---->  main.c
        interrupts.c
        non_vol.c
        program_nv.c
        module_testing.c
        common.h
        common_type.h
        FixedIconDefinitions.h
        Implementation.h
        main.h
        module_testing.h
        non_vol.h
        nonvol_data.h
        
 ---->  Low Level (Directory for 'drop in' low level component)
        ---->   Docs (Directory for documents related to the 'drop in' component of the LL HMI)
        ---->   Core (Directory for source/header files used by both LL & HL HMI)
                    data_handling.c
                    driver.c
                    globals.c
                    data_handling.h
                    driver.h
                    globals.h
                    common_defs.h
                    common_structs.h
                    lcd_patterns.h
        ---->   Implementation (Directory for managing different hardware implementations)
                    implementation_manager.h
                ---->   Common (Directory for managing functions/macros common to multiple implementations)
                        ---->   Functions (Common Functions Directory)
                                    build_image_european.h
                                    common_functions.h
                                    draw_icon.h
                                    get_font_pattern.h
                                    scale_font_2.h
                                    scale_font_4.h
                                    scale_font_5.h
                                    transmit_image.h
                        ---->   Macros (Common Macros Directory)
                                    common_size_macros.h
                                    common_type_macros.h
                ---->   Hardware (Directory to manage different hardware implementations)
                            hmi_hardware_control.h
                            hmi_hw_description_1.h
                            ...
                            hmi_hw_description_36.h
                ---->   Size Macros (Directory to manage size specific macros)
                            macros_size_a.h
                            macros_size_b.h
                            macros_size_c.h
                            macros_size_d.h
                            macros_size_e.h
                ---->   Type Macros (Directory to manage type specific macros)
                            macros_type_1.h
                            macros_type_2.h
                            macros_type_3.h
                            macros_type_4.h
                            macros_type_5.h
                ---->   Type Functions (Directory to manage type specific funtions)
                            functions_type_1.h
                            functions_type_2.h
                            functions_type_3.h
                            functions_type_4.h
                            functions_type_5.h
                ---->   Version (Directory to manage version numbers)
                            version_control.h
                            version_size_a.h
                            version_size_b.h
                            version_size_c.h
                            version_size_d.h
                            version_size_e.h
                            